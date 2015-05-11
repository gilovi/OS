#include "block.h"
#include <cstring>

Block::Block():
_data(nullptr),_dataLength(0),_hashed_data(nullptr),_parent(nullptr),_parentNum(-1),_block_num(0),_toLongest(false),_successor(true),_wasAdded(true)
{
    pthread_mutex_init(&_contender, NULL);
}

Block::Block(char* data, int length , Block* father): //TODO: update to include all members
_data(data),_dataLength(length),_hashed_data(nullptr),_parent(father),_parentNum(father->getNum()),_block_num(-1),_toLongest(false),_successor(false),_wasAdded(false)
{
    _data = new char[length];
    strncpy(data,_data, length); //TODO: confirm this is OK
    pthread_mutex_init(&_contender, NULL);

}

Block::~Block()
{
    delete _data;
    pthread_mutex_destroy(&_contender);
    //TODO: dtor
}

int Block::getDataLength()
{
    return _dataLength;
}

char* Block::getData()
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

