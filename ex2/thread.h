/*
 * thread.h
 *
 * represents a thread in uthreads library.
 * holds relevant data.
 *
 *  Created on: Mar 19, 2015
 *      Author: moshemandel
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <setjmp.h>
#include "general.h"


#ifdef __x86_64__
/* code for 64 bit Intel arch */
	typedef unsigned long address_t;
	#define JB_SP 6
	#define JB_PC 7

#else
/* code for 32 bit Intel arch */
	typedef unsigned int address_t;
	#define JB_SP 4
	#define JB_PC 5
#endif

address_t translate_address(address_t addr);

class Thread
{
public:

//constructors
	Thread(int id, Priority priority);

	Thread(int id ,void (*location)(void), Priority priority);

//	destructor
	~Thread();

	int getID() const;

//	get thread stack (which holds current data/registers of thread)
	char* getStack() const;

//	get thread state (as set by sigsetjmp)
	sigjmp_buf* getThreadState() ;

//	return number of quantums where thread was active
	int getQuantums() const;

//	increase by 1
	void increaseQuantum();

//	return priority (RED,ORANGE,GREEN)
	Priority getPriority() const;

//	set state (READY, RUNNING, BLOCKED, TERMINATED)
	void setState(State state);

//	return state (READY, RUNNING, BLOCKED, TERMINATED)
	State getState() const;


private:
	int _id;
	char *_stack;
	sigjmp_buf _threadState;
	int _quantums;
	Priority _priority;
	State _currState;
};


#endif /* THREAD_H_ */
