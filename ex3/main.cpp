/*
 * main.cpp
 *
 *  Created on: May 6, 2015
 *      Author: moshemandel
 */

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <string>
#include <vector>
#include "blockchain.h"
#include "general.h"
#include "hash.h"

using namespace std;

int testInit()
{
	return init_blockchain();
}

int testAddBlock(const char* src)
{
	int length = sizeof(src);
	char* test = new char[length];
	strncpy(test,src,length);
	int ret =  add_block(test, int(sizeof test));
	delete test;
	return ret;
}

void addMultiple(int num, vector<int>* blockNums)
{
	for (int i=0;i<num;i++)
	{
		int numBlock = testAddBlock("test");
		blockNums->push_back(numBlock);
		cout<< "tried adding block #" << numBlock <<endl;

	}
}

int main()
{
	vector<int> blockNums;
	cout<<"***Testing ex3***"<<endl;
	cout << "testing init, should print " << SUCCESS << " " << testInit() << endl;
//	int nonce = generate_nonce(1,0);
//	char* data = "hash";
//	char* hash = generate_hash(data,sizeof(data),nonce);
//	cout<<"testing hash: " << hash << endl;

	addMultiple(5, &blockNums);
	for(int i = 1; i < blockNums.size() + 1; i++)
	{
		while(was_added(i) == 0)
		{

		}
		cout << i << " was added " <<endl;
	}

}



