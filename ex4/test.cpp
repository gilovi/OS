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
#include <stdio.h>

void print(char* buffer, int size, int offset)
{
	for (int i=offset;i<offset+size;i++)
	{
		if (buffer[i]=='\0')
		{
			printf("*");
		}
		else
		{
			printf("%c", buffer[i]);
		}
	}
	printf("\n");
}

int testCache()
{
	size_t blockSize = 6;
	int numOfBlocks = 2;
	Cache cache = Cache(blockSize, numOfBlocks);
	const char* path1 = "./testRoot/1";
	int fh1 = open(path1,O_RDONLY);
	const char* path2 = "./testRoot/2";
	int fh2 = open(path2,O_RDONLY);
	char buf[1024];
	std::memset(buf,'-',sizeof buf);
	int ret1 = cache.read(path1,buf+2,2,2,fh1);
//	std::cout<<buf<<std::endl;
//	printf("%18s\n",buf);
//	std::cout.write(buf,6);
//	std::cout<<"\n";
	print(buf,4,0);
	int ret2 = cache.read(path1,(buf+4),6,4,fh1);
//	std::cout<<buf<<std::endl;
//	printf("%18s\n",buf);
//	std::cout.write(buf,12);
//	std::cout<<"\n";
	print(buf,10,0);
	int ret3 = cache.read(path1,(buf+8),7,7,fh1);
//	std::cout<<buf<<std::endl;
//	printf("%18s\n",buf);
//	std::cout.write(buf,17);
//	std::cout<<"\n";
	print(buf,18,0);

	int ret4 = cache.read(path1,(buf+18),18,0,fh1);
//	std::cout<<buf<<std::endl;
//	printf("%18s\n",buf);
//	std::cout.write(buf,40);
	print(buf,40,0);
	return 0;
}

int main()
{
	std::cout<<"testing cache"<<std::endl;
	testCache();
	std::cout<<"done testing cache."<<std::endl;
	return 0;
}

