#include "block.h"
#include <cstring>
#include <iostream>

Block::Block():
_data(nullptr),_dataLength(0),_hashed_data(nullptr),_parent(nullptr),_parentNum(-1),_block_num(0),_toLongest(false),_successor(true),_wasAdded(true),_depth(1),_toPrune(false)
{
    pthread_mutex_init(&_contender, NULL);
}

Block::Block(char* data, int length , Block* father): //TODO: update to include all members
_data(nullptr),_dataLength(length),_hashed_data(nullptr),_parent(father),_parentNum(father->getNum()),_block_num(-1),_toLongest(false),_successor(false),_wasAdded(false), _toPrune(true)
{
    _data = new char[length];
    memcpy(_data,data, length); //TODO: confirm this is OK
    pthread_mutex_init(&_contender, NULL);
    _depth = father->getDepth() + 1;
}

Block::~Block()
{
//	std::cout<<"in dtor of block#" << this->getNum() << std::endl;
//	if (_data)
//	{
//		delete[] _data;
//	}
//    if (_hashed_data)
//    {
//    	delete _hashed_data;
//    }
	delete[] _data;
	free(_hashed_data);
    pthread_mutex_destroy(&_contender);
    //TODO: dtor
}

int Block::getDataLength()
{
    return _dataLength;
}

const char* Block::getData()
{
    return _data;
}

void Block::setHash(char* data)
{
//	std::cout<<"data: " << data<<std::endl;
//	std::cout<< "data size: "<<strlen(data)<<std::endl;
//	_hashed_data = new char[strlen(data)+1];
	_hashed_data = data;
//	memcpy(_hashed_data,data,strlen(data)+1);
}

char* Block::getHash()
{
    return _hashed_data;
}

bool Block::isSuccessor()
{
//	genesis block has no parent!
	if (_block_num == 0)
	{
		return true;
	}
    if (pthread_mutex_trylock(&(_parent->_contender)) == 0) // first one to get this lock is the successor. anyone else, not.
    {
        _successor = true;
    }
    return _successor;
}

void Block::setFather(Block* newFather)
{
    _parent = newFather;
}

int Block::getFatherNum()
{
    return _parentNum;
}

void Block::setFatherNum(int num)
{
    _parentNum = num;
}


Block* Block::getFather()
{
    return _parent;
}
int Block::getNum()
{
    return _block_num;
}
void Block::setNum(int num)
{
    _block_num = num;
}
bool Block::getToLongest()
{
    return _toLongest;
}
void Block::setToLongest()
{
    _toLongest = true;
}

void Block::setWasAdded()
{
    _wasAdded = true;
}
bool Block::getWasAdded()
{
    return _wasAdded;
}

int Block::getDepth()
{
	return _depth;
}

bool Block::toPrune()
{
	return _toPrune;
}

void Block::setToPrune(bool toPrune)
{
	_toPrune = toPrune;
}

