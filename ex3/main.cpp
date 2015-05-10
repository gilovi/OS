/*
 * main.cpp
 *
 *  Created on: May 6, 2015
 *      Author: moshemandel
 */

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include "blockchain.h"
#include "general.h"
#include "hash.h"
#include <pthread.h>

using namespace std;

struct ThreadData {
	char* _data;
	int _blockNum;
	int _length;
	ThreadData(char* data, int length)
	{
		_data = new char[length];
		memcpy(data,_data,length);
		_blockNum = 0;
		_length = length;
	}
};



int testInit()
{
	return init_blockchain();
}

int testAddBlock(char* src)
{
	int length = sizeof(src);
	char* test = new char[length];
	strncpy(test,src,length);
	int ret =  add_block(test, int(sizeof test));
	delete test;
	return ret;
}

void addMultiple(int num, vector<ThreadData*>* blocks)
{
	for (int i=0;i<num;i++)
	{
		char buffer[50];
		int length = sprintf(buffer,"thread# %d", i);
		ThreadData* myData = new ThreadData(buffer,length);
		myData->_blockNum = add_block(myData->_data,myData->_length	);
		blocks->push_back(myData);
		cout<< "tried adding block #" << myData->_blockNum <<endl;
		usleep(1000000);
	}
}

void* addThreadBlock(void* threadArg)
{
	struct ThreadData* myData = (struct ThreadData*)threadArg;
	myData->_blockNum = add_block(myData->_data,myData->_length	);
	while (0 == was_added(myData->_blockNum) )
	{

	}
	cout << "           " << myData->_blockNum << " was added " <<endl;
	cout << "chain size: " << chain_size() << endl;
	pthread_exit(NULL);
}

void createThreads(int start, int end, pthread_t* threads,pthread_attr_t* attrPtr, vector<ThreadData*>* blocks)
{
	int rc;
	for (int i=start; i<end;i++)
	{
		char buffer[50];
		int length = sprintf(buffer,"thread# %d", i);
		ThreadData* myData = new ThreadData(buffer,length);
		rc = pthread_create(&threads[i],attrPtr,addThreadBlock,(void*)myData);
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
		blocks->push_back(myData);
	}
}



int main()
{
	vector<ThreadData*> blocks;
	map<int, int> added;
	int num = 10;
	cout<<"***Testing ex3***"<<endl;
	cout << "testing init, should print " << SUCCESS << " " << testInit() << endl;
//	int nonce = generate_nonce(1,0);
//	char* data = "hash";
//	char* hash = generate_hash(data,sizeof(data),nonce);
//	cout<<"testing hash: " << hash << endl;

//	adding multiple blocks in sequence
	{
	//	addMultiple(5, &blocks);
	//	int size = 0;
	//	while (size < chain_size() )
	//	{
	//		for(int i = 0; i < blocks.size(); i++)
	//		{
	//			if (was_added(blocks[i]->_blockNum) == 0)
	//			{
	//
	//			}
	//			else if(added[blocks[i]->_blockNum] == 0)
	//			{
	//				cout << "           " << blocks[i]->_blockNum << " was added " <<endl;
	//				added[blocks[i]->_blockNum] = 1;
	//				cout << "chain size: " << chain_size() << endl;
	//			}
	//		}
	//		size++;
	//	}
	}

//	testing threads
	{
		pthread_t threads[num];
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		createThreads(0,5, threads,&attr,&blocks);
		usleep(3000000);
		createThreads(5,num, threads,&attr,&blocks);
		pthread_attr_destroy(&attr);


		for(int t=0; t<num; t++)
		{
			int rc = pthread_join(threads[t], NULL);
			if (rc)
			{
				cout <<"ERROR; return code from pthread_join() " << rc << endl;
				exit(-1);
			}
		}
		cout << "chain size: " << chain_size() << endl;
		cout << "vector size: " << blocks.size() << endl;
	}

	map<int, Block*> leaves = getLeaves();
	for (map<int, Block*>::iterator it = leaves.begin();it != leaves.end();++it)
	{
		stack<int> chain;
		Block* tmp = it->second;
		while(tmp->getNum() != 0)
		{
			chain.push(tmp->getNum() );
			tmp = tmp->getFather();

		}
		cout << "0";
		while(!chain.empty())
		{
			cout<<" -> "<<chain.top();
			chain.pop();
		}
		cout<< endl;
	}
	pthread_exit(NULL);
}



