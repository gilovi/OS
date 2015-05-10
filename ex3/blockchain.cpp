/*
 * blockchain.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: moshemandel
 */
#include <errno.h>
#include <pthread.h>
#include "general.h"
#include <list>
#include <vector>
#include <queue>
#include <unistd.h>
#include <map>
#include "block.h"
#include "hash.h"
#include <iostream>


enum closeStatus {OPEN,CLOSING,CLOSED};
//pthread_mutex_t gStatusLock;
closeStatus gStatus = CLOSED;


Block* gCurrFather;

pthread_mutex_t gBlocksQueueLock;
std::list<Block*> gBlocksQueue;

pthread_mutex_t gBlocksLock;
std::map<int,Block*> gBlocks;

int gLongestChainSize;

pthread_mutex_t gblocksNumLock;
int gblocksNum;

pthread_mutex_t gAvailNumLock;
std::priority_queue<int, std::vector<int>, std::greater<int> > gAvailNum;

std::map<int, Block*> gLeaves;


pthread_t _daemon , _closingTh;

void* daemonFunc(void*)
{
    while (true)
    {
    usleep(2);

        if (gBlocksQueue.empty())
        {
            if (gStatus != OPEN)
            {
                break;
            }
            usleep(5);
        }
        else // there are blocks waiting fof hashing
        {
            pthread_mutex_lock (&gBlocksQueueLock);
            Block* toCompute = gBlocksQueue.front();
            gBlocksQueue.pop_front();
            pthread_mutex_unlock (&gBlocksQueueLock);
            if (gStatus == OPEN)
            {
                if(toCompute->getToLongest())
                {
                toCompute->setFather(gCurrFather);
                toCompute->setFatherNum(gCurrFather->getNum());
                }
                int nonce = generate_nonce(toCompute->getNum(), toCompute->getFatherNum()) ;
                toCompute->setHash(generate_hash(toCompute->getData() ,toCompute->getDataLength() , nonce));
                if(toCompute->isSuccessor())
                {
                    gLongestChainSize++;
                    gCurrFather = toCompute;
                }
                toCompute->setWasAdded();
                pthread_mutex_lock(&gblocksNumLock);
                gblocksNum++;
                pthread_mutex_unlock(&gblocksNumLock);
            }

            else // we are closing.
            {
                int nonce = generate_nonce(toCompute->getNum(), toCompute->getFatherNum()) ;
                std::cout << generate_hash(toCompute->getData() ,toCompute->getDataLength() , nonce) << std::endl;
            }

//            maintaining leaves list
            int fatherNum = toCompute->getFather()->getNum();
            if(gLeaves.count(fatherNum ) )
            {
            	gLeaves.erase(fatherNum);
            }
            gLeaves[toCompute->getNum() ] = toCompute;

            pthread_mutex_lock(&gblocksNumLock);
            gblocksNum++;
            pthread_mutex_unlock(&gblocksNumLock);
        }

    }
    return 0;
}

int init_blockchain()
{
    if (gStatus == CLOSING)
    {
        return FAILURE;
    }
    gCurrFather = new Block(); // the genesis
    init_hash_generator();

    pthread_mutex_init(&gBlocksQueueLock, NULL);
    pthread_mutex_init(&gBlocksLock, NULL);
    pthread_mutex_init(&gAvailNumLock, NULL);
    pthread_mutex_init(&gblocksNumLock, NULL);


    gBlocks[0] = gCurrFather;
    gLongestChainSize = 1;


    //pthread_attr_t tattr;
    //if(pthread_attr_init(&tattr))
    //{return FAILURE;}//TODO: add error handle
    //if(pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED))
    //{return FAILURE;}//TODO: add error handle
    if(pthread_create(&_daemon, NULL, daemonFunc, NULL)) //creat it joinable so we can know if it was finished on close
    {return FAILURE;}//TODO: add error handle
    //pthread_attr_destroy(&tattr);

    gStatus = OPEN;
    return SUCCESS;

}

int add_block(char *data , int length)
{
    if (gStatus == CLOSING)
    {
        return FAILURE;
    }
    Block* newBlock = new Block(data, length , gCurrFather);

    pthread_mutex_lock (&gBlocksQueueLock);
    gBlocksQueue.push_back(newBlock);
    pthread_mutex_unlock (&gBlocksQueueLock);

    int num;
    if (gAvailNum.empty())
    {
        pthread_mutex_lock (&gBlocksLock); // find the lowst (new) unused place & occupy it
        num = gBlocks.size();
        gBlocks[num] = newBlock;
        pthread_mutex_unlock (&gBlocksLock);
    }
    else
    {
        pthread_mutex_lock (&gAvailNumLock); //get the freed number safly
        num = gAvailNum.top();
        gAvailNum.pop();
        pthread_mutex_unlock (&gAvailNumLock);

        pthread_mutex_lock (&gBlocksLock);//add the new block to the place safly
        gBlocks[num] = newBlock;
        pthread_mutex_unlock (&gBlocksLock);

    }

    newBlock->setNum(num);
    return num;

}

int to_longest(int block_num)
{

    pthread_mutex_lock (&gBlocksLock);
    if (gBlocks.count(block_num) > 0)
    {
        Block* block = gBlocks[block_num];

        if (!block->getWasAdded())
        {//wasnt added. set to longest!
            block->setToLongest();
            pthread_mutex_unlock (&gBlocksLock);
            return 0;
        }
        pthread_mutex_unlock (&gBlocksLock);
        return 1;//was added

    }
    else //block dosent exist
    {
        pthread_mutex_unlock (&gBlocksLock);
        return -2;
    }
    return FAILURE;

}


int attach_now(int block_num)
{
    if (gStatus == CLOSING)
    {
        return FAILURE;
    }
    pthread_mutex_lock (&gBlocksQueueLock);
    for (std::list<Block*>::iterator it = gBlocksQueue.begin(); it != gBlocksQueue.end(); it++)
    {
        if ((*it)->getNum() == block_num) // block found in 'to add' queue (wasnt added yet)
        {
            gBlocksQueue.erase(it);
            gBlocksQueue.push_front(*it);
            pthread_mutex_unlock (&gBlocksQueueLock);
            return 0;
        }

    }//its not in the queue.
    pthread_mutex_unlock (&gBlocksQueueLock);
    pthread_mutex_lock (&gBlocksLock);
	if (gBlocks.count(block_num) == 0) // check if block exists
	{
        pthread_mutex_unlock (&gBlocksLock);
        return -2; //nope

	}
	else //it exist
	{
        if (gBlocks[block_num]->getWasAdded())
        pthread_mutex_unlock (&gBlocksLock);
        return 0;
	}


    return FAILURE;

}

int was_added(int block_num)
{
	if (0 == gBlocks.count(block_num) )
	{
		return NON_EXIST;
	}
	int ret = gBlocks[block_num]->getWasAdded();
//	TODO: if error?? than return -1, what error can happen?
	return ret;
}

int chain_size()
{
    pthread_mutex_lock (&gblocksNumLock);
    int ret = gblocksNum;
    pthread_mutex_unlock (&gblocksNumLock);
    return ret;
}

int prune_chain()
{
    for (std::map<int,Block*>::iterator it = gBlocks.begin(); it != gBlocks.end(); ++it)
    {
        if (!(it->second->isSuccessor()))
        {
            delete it->second;
        }
    }
    return SUCCESS;

}

void* closeFunc(void*)
{
std::list<Block*> notAdded;

for (std::map<int,Block*>::iterator it = gBlocks.begin(); it != gBlocks.end(); ++it)
    {
        if (it->second->getWasAdded())
        {
            delete it->second; //delete add attached blocks
        }
        else
        {
            notAdded.push_back(it->second);
        }

    }
    pthread_join(_daemon, NULL); // wait for _daemon to finish hashing
    //relese remains
    for (std::list<Block*>::iterator it = notAdded.begin(); it != notAdded.end(); ++it)
    {
        delete *it;
    }
    close_hash_generator();
    pthread_mutex_destroy(&gBlocksQueueLock);
    pthread_mutex_destroy(&gBlocksLock);
    pthread_mutex_destroy(&gAvailNumLock);
    pthread_mutex_destroy(&gblocksNumLock);

    gStatus = CLOSED;
    return 0;
}

void close_chain()
{
    gStatus = CLOSING;
    if(pthread_create(&_closingTh, NULL, closeFunc, NULL))
    {
        exit(1);
    }
}

int return_on_close()
{
    int ret = pthread_join(_closingTh, NULL);
	switch (ret)
	{
	case 0:
	return 1;

	case ESRCH:
        return -2;

    default:
        return -1;
    }

}

std::map<int, Block*> getLeaves()
{
	return gLeaves;
}
