/*
 * thread.cpp
 *
 *  Created on: Mar 19, 2015
 *      Author: moshemandel
 */
#include "thread.h"

Thread::Thread(int id)
: _id(id), _stack(0), _threadState(0), _quantums(0), _priority(GREEN)
{

}

Thread::~Thread()
{

}

int Thread::getID() const
{
	return _id;
}

char* Thread::getStack() const
{
	return _stack;
}

sigjmp_buf* Thread::getThreadState() const
{
	return _threadState;
}

int Thread::getQuantums() const
{
	return _quantums;
}

void Thread::increaseQuantum()
{
	_quantums++;
}

Priority Thread::getPriority() const
{
	return _priority;
}

