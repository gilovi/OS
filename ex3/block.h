#ifndef BLOCK_H
#define BLOCK_H

#include <pthread.h>
#include <string>
/**
*   A class that represents a block in the chain
*   and holds the relevant data needed.
*/
class Block
{
    public:
        Block();
        Block(char* data, int length , Block* parent);
         ~Block();

        const char* getData();
        int getDataLength();
        void setHash(char* data);
        char* getHash();
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
        void setWasHashed(bool);
        bool getWasHashed();
        int getDepth();
        bool toPrune();
        void setToPrune(bool toPrune);

    protected:

    private:
        char* _data;
        int _dataLength;
        char * _hashed_data;

        Block* _parent;
        int _parentNum;
        int _block_num;
        bool _toLongest;

        bool _wasAdded;
        bool _wasHashed;
        int _depth;
        bool _toPrune;
};

#endif // BLOCK_H
