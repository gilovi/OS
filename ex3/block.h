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

        void setHash(char* data);
        char* getHash();
        bool isSuccessor();
        void setFather(Block*);
        Block* getFather();
        int getNum();
        void setNum(int);
        bool getToLongest();
        void setToLongest();

    protected:

    private:
        std::string _data;
        char * _hashed_data;
        pthread_mutex_t _contender;
        Block* _parent;
        int _block_num;
        bool _toLongest;
        bool _successor;
};

#endif // BLOCK_H
