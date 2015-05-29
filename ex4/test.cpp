/*
 * test.cpp
 *
 *  Created on: May 29, 2015
 *      Author: moshemandel
 */

#include "cache.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>

int testCache()
{
	size_t blockSize = 17;
	int numOfBlocks = 1;
	Cache cache = Cache(blockSize, numOfBlocks);
	const char* path1 = "./testRoot/1";
	int fh1 = open(path1,O_RDONLY);
	const char* path2 = "./testRoot/2";
	int fh2 = open(path2,O_RDONLY);
	char buf[1024*2];
	std::memset(buf,0,sizeof buf);
	int ret1 = cache.read(path2,buf,blockSize,0,fh2);
	std::cout<<buf<<std::endl;
	int ret2 = cache.read(path1,buf+blockSize,blockSize,0,fh1);
	std::cout<<buf<<std::endl;


	return 0;
}

int main()
{
	std::cout<<"testing cache"<<std::endl;
	testCache();
	std::cout<<"done testing cache."<<std::endl;
	return 0;
}

