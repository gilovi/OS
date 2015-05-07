/*
 * blockchain.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: moshemandel
 */

#include <pthread.h>
#include "general.h"
#include <list>
#include <vector>
#include <queue>
#include <unistd.h>
#include <map>
#include "block.h"
#include "hash.h"


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


pthread_t _daemon;

void* daemonFunc(void*)
{
    while (true)
    {
    usleep(2);

        if (gBlocksQueue.empty())
        {
            usleep(5);
        }
        else
        {
            pthread_mutex_lock (&gBlocksQueueLock);
            Block* toCompute = gBlocksQueue.front();
            gBlocksQueue.pop_front();
            pthread_mutex_unlock (&gBlocksQueueLock);
            if(toCompute->getToLongest())
            {
               toCompute->setFather(gCurrFather);
            }
            int nonce = generate_nonce(toCompute->getNum(), toCompute->getFather()->getNum()) ;
            toCompute->setHash(generate_hash(toCompute->getData() ,toCompute->getDataLength() ,  nonce)); //toCompute.getData().length() ???
            if(toCompute->isSuccessor())
            {
                gLongestChainSize++;
                gCurrFather = toCompute;
            }
            toCompute->wasAdded();
            pthread_mutex_lock(&gblocksNumLock);
            gblocksNum++;
            pthread_mutex_unlock(&gblocksNumLock);

        }

    }
}

int init_blockchain()
{

    gCurrFather = new Block(); // the genesis
    init_hash_generator();

    pthread_mutex_init(&gBlocksQueueLock, NULL);
    pthread_mutex_init(&gBlocksLock, NULL);
    pthread_mutex_init(&gAvailNumLock, NULL);
    pthread_mutex_init(&gblocksNumLock, NULL);


    gBlocks[0] = gCurrFather;
    gLongestChainSize = 1;

    pthread_attr_t tattr;
    if(pthread_attr_init(&tattr))
    {return FAILURE;}//TODO: add error handle
    if(pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED))
    {return FAILURE;}//TODO: add error handle
    if(pthread_create(&_daemon, &tattr, daemonFunc, NULL))
    {return FAILURE;}//TODO: add error handle
    pthread_attr_destroy(&tattr);

    return SUCCESS;

}

int add_block(char *data , int length)
{
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
    gBlocks[block_num]->setToLongest();
    pthread_mutex_unlock (&gBlocksLock);
    //TODO: add failure/success cases
    return FAILURE;

}


int attach_now(int block_num)
{
    pthread_mutex_lock (&gBlocksQueueLock);
    for (std::list<Block*>::iterator it = gBlocksQueue.begin(); it != gBlocksQueue.end(); it++)//TODO: find the block with blocknum in blocksQueue get it ,delete it and push it to the top of the list
    {
        if ((*it)->getNum() == block_num)
        {
            gBlocksQueue.erase(it);
            gBlocksQueue.push_front(*it);
            pthread_mutex_unlock (&gBlocksQueueLock);
            return 0;
        }

    }
    pthread_mutex_unlock (&gBlocksQueueLock);
	//if we got here, so block_num is not in the queue.

    return FAILURE;

}

int was_added(int block_num)
{
	return gBlocks[block_num]->getWasAdded();
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
    //TODO: should be a simple loop on the blocks map (just verify that it is not a successor and that it was added..)
    return FAILURE;

}

void close_chain()
{

}

int return_on_close()
{
	return FAILURE;
}
