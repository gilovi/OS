#include "block.h"
#include <cstring>
#include <iostream>

Block::Block():
_data(nullptr),_dataLength(0),_hashed_data(nullptr),_parent(nullptr),_parentNum(-1),_block_num(0),_toLongest(false),_wasAdded(true),_wasHashed(false),_depth(1),_toPrune(false)
{

}

Block::Block(char* data, int length , Block* father): //TODO: update to include all members
_data(nullptr),_dataLength(length),_hashed_data(nullptr),_parent(father),_parentNum(father->getNum()),_block_num(-1),_toLongest(false),_wasAdded(false),_wasHashed(false), _toPrune(true)
{
    _data = new char[length];
    memcpy(_data,data, length);
    _depth = father->getDepth() + 1;
}

Block::~Block()
{

	delete[] _data;
	free(_hashed_data);

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

	_hashed_data = data;

}

char* Block::getHash()
{
    return _hashed_data;
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

void Block::setWasHashed(bool hashed)
{
	_wasHashed = hashed;
}

bool Block::getWasHashed()
{
	return _wasHashed;
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

