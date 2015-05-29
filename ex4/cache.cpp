/*
 * Cache.cpp
 *
 *  Created on: May 27, 2015
 *      Author: moshemandel
 */

#include <cmath>
#include <cstring>
#include <unistd.h>
#include "cache.h"

Cache::Cache(size_t blockSize, int numOfBlocks)
	: _blockSize(blockSize), _numOfBlocks(numOfBlocks)
{

}

Cache::~Cache()
{
// clear map and queue
	std::map<std::string, Block*>::iterator it;
	for (it = _blocks.begin();it != _blocks.end(); ++it)
	{
		delete it->second;
		_blocks.erase(it);
	}
	while(!_blocksQueue.empty() )
	{
		_blocksQueue.pop();
	}
}

//assume path is already converted relative to rootdir
int Cache::read(const char *path, char *buf, size_t size, off_t offset,
		uint64_t fh)
{
// if block in cache: retrieve from cache and increase counter,
//		otherwise: read relevant path from disk and write to cache.
	int requiredNumOfBlocks = ceil(double(size)/double(_blockSize));
	int firstBlockNum = ceil(double(offset)/double(_blockSize));
	if (0 == firstBlockNum)
	{
		firstBlockNum = 1;
	}
	int lastBlockNum = firstBlockNum + requiredNumOfBlocks -1;
	int currBlockNum = firstBlockNum;
//	traverse over all blocks
	while(currBlockNum <= lastBlockNum)
	{
		std::string id = getID(path, currBlockNum);
//		if not in cache: write to cache
		if (!_blocks.count(id) )
		{
			write(path,currBlockNum,fh);
		}

//		if current Block is last block load only bytes remained
//		otherwise -- load all block (_blockSize)
		size_t currBlockSize;
		if (currBlockNum == lastBlockNum)
		{
			currBlockSize = size - (requiredNumOfBlocks - 1)*_blockSize;
		}
		else
		{
			currBlockSize = _blockSize;
		}
		std::memcpy(buf+(currBlockNum-1)*_blockSize,_blocks[id]->_buf, currBlockSize);
		_blocks[id]->_counter++;

		currBlockNum++;
	}

//	TODO: handle errors and return appropriate value
	return 0;
}

int Cache::write(const char* path, int blockNum,uint64_t fh)
{
	int retstat = 0;
	if (isFull() )
	{
		Block* LFU = _blocksQueue.top();
		_blocksQueue.pop();
		std::string id = LFU->_id;
		delete _blocks[id];
		_blocks.erase(id);
	}


	Block* newBlock = new Block(path,blockNum,_blockSize);
	_blocks[newBlock->_id] = newBlock;
	_blocksQueue.push(newBlock);
//		TODO: should a case of EOF require allocating only the needed space and not the full block size?
	retstat = pread(fh, _blocks[newBlock->_id]->_buf, _blockSize, (blockNum-1)*_blockSize);



	return retstat;
}

bool Cache::isFull()
{
	return (int)_blocks.size() == _numOfBlocks;
}

std::string Cache::getID(const char* path, int blockNum)
{
	std::ostringstream ostr; //output string stream
	ostr << blockNum;
	std::string id = (std::string)path + "//" + ostr.str();
	return id;
}
