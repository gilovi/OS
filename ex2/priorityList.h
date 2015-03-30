/*
 * ready.h
 *
 *  Created on: Mar 29, 2015
 *      Author: moshemandel
 */

#ifndef READY_H_
#define READY_H_
#include <list>
#include <map>
#include "thread.h"

struct threadData {
	Priority priority;
	std::list<Thread*>::iterator threadIt;
};

class PriorityList
{
public:

	PriorityList();

	~PriorityList();

	bool empty() const;

	Thread* pop();

	void push(Thread* thread);

	void remove(int tid);

	int size() const;

	bool contains(int tid);

private:

	std::list<Thread*> _red;
	std::list<Thread*> _orange;
	std::list<Thread*> _green;
	std::map<int, threadData> _idMap;
};




#endif /* READY_H_ */
