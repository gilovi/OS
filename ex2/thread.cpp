/*
 * thread.cpp
 *
 *  Created on: Mar 19, 2015
 *      Author: moshemandel
 */

#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include "thread.h"



Thread::Thread(int id, Priority priority)
: _id(id), _quantums(0), _priority(priority), _currState(READY)
{
    _stack = new char[STACK_SIZE];
}

Thread::Thread(int id ,void (*location)(void), Priority priority)
: _id(id), _quantums(0), _priority(priority), _currState(READY)
{
    _stack = new char[STACK_SIZE];

    address_t sp, pc;
    sp = (address_t)_stack + STACK_SIZE - sizeof(address_t);
    pc = (address_t)location;
    sigsetjmp(_threadState, 1);
    (_threadState->__jmpbuf)[JB_SP] = translate_address(sp);
    (_threadState->__jmpbuf)[JB_PC] = translate_address(pc);
    if (sigemptyset(&_threadState->__saved_mask) == FAILURE)
    {
		std::cerr << SIGEMPTY_ERR << strerror(EINVAL) << std::endl;
		exit(1);
	}



}

Thread::~Thread()
{
    delete _stack;
}

int Thread::getID() const
{
	return _id;
}

char* Thread::getStack() const
{
    return _stack;
}

sigjmp_buf* Thread::getThreadState()
{
	return &_threadState;
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

void Thread::setState(State state)
{
	_currState = state;
}

State Thread::getState() const
{
	return _currState;
}

