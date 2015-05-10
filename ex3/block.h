#ifndef BLOCK_H
#define BLOCK_H

#include <pthread.h>
#include <string>

class Block
{
    public:
        Block();
        Block(char* data, int length , Block* parent);
         ~Block();

        char* getData();
        int getDataLength();
        void setHash(char* data);
        char* getHash();
        bool isSuccessor();
        void setFather(Block*);
        Block* getFather();
        int getFatherNum();
        void setFatherNum(int);
        int getNum();
        void setNum(int);
        bool getToLongest();
        void setToLongest();
        void setWasAdded();
        bool getWasAdded();
    protected:

    private:
        char* _data;
        int _dataLength;
        char * _hashed_data;
        pthread_mutex_t _contender;
        Block* _parent;
        int _parentNum;
        int _block_num;
        bool _toLongest;
        bool _successor;
        bool _wasAdded;
};

#endif // BLOCK_H
