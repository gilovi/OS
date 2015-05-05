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

Block* gCurrFather;

pthread_mutex_t gBlocksQueueLock;
std::list<Block*> gBlocksQueue;

pthread_mutex_t gBlocksLock;
std::map<int,Block*> gBlocks;

int gLongestChainSize;

pthread_mutex_t gblocksNumLock;
int gblocksNum;

pthread_mutex_t gAvailNumLock;
std::priority_queue<int, std::vector<int>, std::greater<int> > gAvailNum


pthread_t _daemon;

void daemonFunc()
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
            Block* toCompute = gBlocksQueue.back();
            gBlocksQueue.pop_back();
            pthread_mutex_unlock (&gBlocksQueueLock);
            if(toCompute->getToLongest())
            {
               toCompute->setFather(gCurrFather);
            }
            toCompute->setHash(generate_nonce(toCompute->getNum(), toCompute->getFather()->getNum());
            if(toCompute->isSuccessor())
            {
                gLongestChainSize++;
                gCurrFather = toCompute;
            }
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
    {}//TODO: add error handle
    if(pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED))
    {}//TODO: add error handle
    if(pthread_create(&_daemon, &tattr, daemonFunc, NULL))
    {}//TODO: add error handle

}

int add_block(char *data , int length)
{
    newBlock = new Block(*data, length , gFathers.back());

    pthread_mutex_lock (&gBlocksQueueLock);
    gBlocksQueue.push_back(newBlock);
    pthread_mutex_unlock (&gBlocksQueueLock)

    if (gAvailNum.empty())
    {
        num = gBlocks.size();
    }
    else
    {
        pthread_mutex_lock (&gAvailNumLock);
        num = gAvailNum.top;
        gAvailNum.pop;
        pthread_mutex_unlock (&gAvailNumLock)

    }
    pthread_mutex_lock (&gBlocksLock);
    gBlocks[num] = newBlock;
    pthread_mutex_unlock (&gBlocksLock);

    return num;

}

int to_longest(int block_num)
{
	return FAILURE;
}


int attach_now(int block_num)
{
	return FAILURE;
}

int was_added(int block_num)
{
	return FAILURE;
}

int chain_size()
{
	return FAILURE;
}

int prune_chain()
{
	return FAILURE;
}

void close_chain()
{

}

int return_on_close()
{
	return FAILURE;
}
