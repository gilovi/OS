/*
 * thread.h
 *
 *  Created on: Mar 19, 2015
 *      Author: moshemandel
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <setjmp.h>

//TODO: this is redefined from uthreads.h, should be fixed
#define MAX_THREAD_NUM 100 /* maximal number of threads */
#define STACK_SIZE 4096 /* stack size per thread (in bytes) */

class Thread
{
public:
	Thread();
	~Thread();

	int getID() const {return _id;}

	char* getStack() const {return _stack;}

	sigjmp_buf* getThreadState() const {return _threadState;}

	int getQuntums() const {return _quantums;}

	void increaseQuantum() {_quantums++;}

	Priority getPriority() const {return _priority;}

private:
	int _id;
	char _stack[STACK_SIZE];
	sigjmp_buf *_threadState;
	int _quantums;
	Priority _priority;
};


#endif /* THREAD_H_ */
