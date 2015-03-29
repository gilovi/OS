/*
 * thread.h
 *
 *  Created on: Mar 19, 2015
 *      Author: moshemandel
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <setjmp.h>
#include "uthreads.h"

//TODO: this is redefined from uthreads.h, should be fixed
#define MAX_THREAD_NUM 100 /* maximal number of threads */
#define STACK_SIZE 4096 /* stack size per thread (in bytes) */

class Thread
{
public:

	Thread(int id, Priority priority);

	Thread(int id ,void (*location)(void), Priority priority);

	~Thread();

	int getID() const;

	char* getStack() const;

	sigjmp_buf* getThreadState() const;

	int getQuantums() const;

	void increaseQuantum();

	Priority getPriority() const;


private:
	int _id;
	char _stack[STACK_SIZE];
	sigjmp_buf _threadState;
	int _quantums;
	Priority _priority;
};


#endif /* THREAD_H_ */
