#include "block.h"
#include <cstring>

Block::Block():
_data(nullptr),_dataLength(0),_hashed_data(nullptr),_parent(nullptr),_block_num(0),_toLongest(false),_successor(true),_wasAdded(true)
{
    pthread_mutex_init(&_contender, NULL);
}

Block::Block(char* data, int length , Block* father): //TODO: update to include all members
_data(data),_dataLength(length),_hashed_data(nullptr),_parent(father),_block_num(-1),_toLongest(false),_successor(false),_wasAdded(false)
{
    _data = new char[length];
    std::memcpy(data,_data, length); //TODO: confirm this is OK
    pthread_mutex_init(&_contender, NULL);

}

Block::~Block()
{
    delete _data;
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

void Block::wasAdded()
{
    _wasAdded = true;
}
bool Block::getWasAdded()
{
    return _wasAdded;
}

