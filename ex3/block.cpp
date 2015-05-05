#include "block.h"


Block::Block():
_data(nullptr),_hashed_data(nullptr),_parent(nullptr),_successor(false)
{
    pthread_mutex_init(&_contender, NULL);
}

Block::Block(char* data, int length , Block* father): //TODO: update to include all members
_data(data, length),_hashed_data(nullptr),_parent(father)
{
    pthread_mutex_init(&_contender, NULL);

}

Block::~Block()
{
    //dtor
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

