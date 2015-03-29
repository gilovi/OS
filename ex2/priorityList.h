/*
 * ready.h
 *
 *  Created on: Mar 29, 2015
 *      Author: moshemandel
 */

#ifndef READY_H_
#define READY_H_
#include <queue>
#include "thread.h"

class PriorityList
{
public:

	PriorityList();

	~PriorityList();

	bool empty() const;

	Thread* pop();

	void push(Thread* val);

private:
	std::queue<Thread*> _red;
	std::queue<Thread*> _orange;
	std::queue<Thread*> _green;
};




#endif /* READY_H_ */
