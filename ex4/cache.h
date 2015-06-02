/*
 * cache.h
 *
 *  Created on: May 27, 2015
 *      Author: moshemandel
 */

#ifndef CACHE_H_
#define CACHE_H_

#include <map>
#include <string>
#include <sstream>
#include <queue>
#include <vector>
#include <functional>
#include <cstdint>

#include <iostream>
class Cache
{
	public:
		Cache(size_t blockSize=0, int numOfBlocks=0);
		~Cache();
		int read(const char *path, char *buf, size_t size, off_t offset,
				uint64_t fh);



	private:



		struct Block{
			Block(const char *path, int blockNum, size_t blockSize)
			{
				_path = path;
				_blockNum = blockNum;
				_counter = 0;

				_buf = new char[blockSize];
				std::ostringstream ostr; //output string stream
				ostr << blockNum;
				_id = _path + "//"  + ostr.str();

			}
			~Block()
			{
				delete[] _buf;
			}
			std::string _path;
			int _blockNum;
			int _counter;
			char* _buf;
			std::string _id;
		};


		struct CmpCounters{
			bool operator()(const Block* lhs, const Block* rhs) const
			{
				if(lhs->_counter != rhs->_counter)
				{
					return lhs->_counter > rhs->_counter;
				}
				else
				{
					return strcmp(lhs->_id.data(),rhs->_id.data() ) < 0;
				}
			}
		};

		bool isFull();
		int write(const char* path, int blockNum, uint64_t fh);
		std::string getID(const char* path, int blockNum);
		void updateQueue(std::string id);

		size_t _blockSize;
		int _numOfBlocks;
		std::map<std::string, Block*> _blocks;
		std::priority_queue<Block*,std::vector<Block*>, CmpCounters> _blocksQueue;

//		TODO: remove. for debugging purposes
		void printMap()
		{
			std::cout<<"print map"<<std::endl;
			std::map<std::string, Block*>::iterator it;
			for (it = _blocks.begin();it != _blocks.end(); ++it)
			{
				std::cout<<it->first <<std::endl;
			}
			std::cout<<"\n+++++\n";
		}
};


#endif /* CACHE_H_ */
