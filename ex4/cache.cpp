/*
 * Cache.cpp
 *
 *  Created on: May 27, 2015
 *      Author: moshemandel
 */

#include <cmath>
#include <cstring>
#include <unistd.h>
#include <stack>
#include <regex>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include "cache.h"



Cache::Cache(size_t blockSize, int numOfBlocks)
	: _blockSize(blockSize), _numOfBlocks(numOfBlocks)
{

}

Cache::~Cache()
{
// clear map and queue
	while(!_blocksQueue.empty() )
	{
		_blocksQueue.pop();
	}
	std::map<std::string, Block*>::iterator it;
	for (it = _blocks.begin();it != _blocks.end(); ++it)
	{
		delete it->second;
//		_blocks.erase(it);
	}
	_blocks.clear();

}

//TODO:assume path is already converted relative to rootdir??? NOT NECESSARY!
int Cache::read(const char *path, char *buf, size_t size, off_t offset,
		uint64_t fh)
{
// if block in cache: retrieve from cache and increase counter,
//		otherwise: read relevant path from disk and write to cache.
	int retstat = 0;
	int firstBlockNum = floor(double(offset)/double(_blockSize));
	int lastBlockNum = floor(double(offset+size-1)/double(_blockSize));
//	int lastBlockNum = firstBlockNum + ceil(size / (double)_blockSize);
	int currBlockNum = firstBlockNum;
	off_t totalOffset = 0;
	memset(buf, 0, size);//set zeros on buf
//	get file size:
	struct stat st ;
	fstat(fh, &st);
	size_t fSize = st.st_size;

//	traverse over all blocks

//	TODO: remove. for debugging purposes
//	std::cout<<"size: " << size
//			<< "\noffset: " << offset
//			<< "\nfirstBlockNum: " << firstBlockNum
//			<< "\nlastBlockNum: " << lastBlockNum
//			<< "\nblocksize: " << _blockSize<< std::endl;

	if(offset > (off_t)fSize) return -ENXIO;

	while(currBlockNum <= lastBlockNum)
	{
		std::string id = getID(path, currBlockNum);

//		if current Block is first block, load only bytes from offset until end of block,
//		until end of block (load partial block)
//		Otherwise - start from beginning of block
//		if current Block is last block, load only bytes remained
//		otherwise -- load all the whole block (_blockSize)
		off_t blockOffset = 0;
		size_t currBlockSize = _blockSize;
		if (currBlockNum == firstBlockNum)
		{
			blockOffset = offset - currBlockNum*_blockSize;
			if (currBlockNum == lastBlockNum)
			{
				currBlockSize = size;
			}
			else
			{
				currBlockSize = _blockSize - blockOffset;
			}
		}
		else if (currBlockNum == lastBlockNum)
		{
			if ( (size-totalOffset)%_blockSize == 0)
			{
				currBlockSize = _blockSize;
			}
			else
			{
				currBlockSize = (size - totalOffset)%_blockSize;
			}
		}
//		else if (totalOffset + currBlockSize > size) {
//			currBlockSize = (size - totalOffset) % _blockSize ? (size - totalOffset) % _blockSize : _blockSize;
//		}

//		std::cout << "blockNumber: " << currBlockNum
//				<< "\nblockOffset: "  << blockOffset
//				<< "\ncurrBlockSize: " << currBlockSize
//				<< "\ntotalOffset: " << totalOffset <<std::endl;

		//		if not in cache: write to cache
		if (!_blocks.count(id) )
		{
			write(path,currBlockNum,fh);
		}

		retstat += currBlockSize;
		if (_blocks.count(id) )
		{
			_blocks[id]->_counter++;
		}
		else
		{
//			TODO: remove?
			std::cout<<"ERROR: id not found"<<std::endl;
		}
//		TODO: remove
//		std::cout<<"Cache::read. id: " << id <<std::endl;
//		update priority queue:
		updateQueue(id);
//		std::cout<<"=========="<<std::endl;
//		std::cout<<"before memcpy.\nblockOffset: " << blockOffset
//					<< "\ncurrBlockSize: " << currBlockSize
//					<< "\ntotalOffset: " << totalOffset
//					<< std::endl;
		std::memcpy(buf+totalOffset,_blocks[id]->_buf + blockOffset, currBlockSize);
//		std::cout<<"after memcpy"<<std::endl;

		totalOffset += currBlockSize;
		currBlockNum++;
	}

//	TODO: handle errors and return appropriate value
	return retstat;
}

//return 1 if file found, 0 otherwise
int Cache::rename(const char *path, const char *newpath)
{
	std::string eStr(path);
	eStr.append("(//[[:d:]]+)");
	std::regex e(eStr);
	std::map<std::string, Block*>::iterator it;
	for (it = _blocks.begin();it!=_blocks.end();++it)
	{
		if(std::regex_match(it->second->_id,e) )
		{
			std::string replaceStr(newpath);
			replaceStr.append("$1");
			std::string newId = std::regex_replace(it->second->_id,e,replaceStr);
			it->second->_id = newId;
			updateQueue(newId);
//			found such file: return 1
			return 1;
		}
	}
//	no file found: return 0
	return 0;
}

int Cache::write(const char* path, int blockNum,uint64_t fh)
{
//	std::cout << "in Cache::write" <<std::endl;
//	std::cout << "blockNum: " << blockNum << std::endl;
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
//	std::cout << "test-----" <<std::endl;
//	std::cout << "newBlock id: " << newBlock->_id << std::endl;
//	std::cout << "pread" << std::endl;
	retstat = pread(fh, _blocks[newBlock->_id]->_buf, _blockSize, blockNum*_blockSize);
//	std::cout<<"done pread"<<std::endl;



	return retstat;
}

void Cache::PrintToLog(std::ofstream& log)
{
	std::map<std::string, Block*>::iterator it;
	for (it = _blocks.begin();it != _blocks.end(); ++it)
	{
		Block* block = it->second;
		log << block->_path.substr(1) << " " << block->_blockNum+1 << " " << block->_counter <<std::endl;
	}
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

void Cache::updateQueue(std::string id)
{
	std::stack<Block*> stk;
	while(!_blocksQueue.empty() )
	{
		Block* tmp = _blocksQueue.top();
		stk.push(tmp);
		_blocksQueue.pop();
		if(tmp->_id == id)
		{
			while(!stk.empty() )
			{
				_blocksQueue.push(stk.top() );
				stk.pop();
			}
			return;
		}
	}

}
