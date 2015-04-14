/*
 * priorityList.h
 * a wrapper for priority list which is used as READY list in uthreads library
 *
 *  Created on: Mar 29, 2015
 *      Author: moshemandel
 */

#ifndef READY_H_
#define READY_H_
#include <list>
#include <map>
#include "thread.h"

//wrapper of thread data
struct threadData {
//	according to which queue the thread belongs to
	Priority priority;
//	iterator of respective queue to thread
	std::list<Thread*>::iterator threadIt;
};

class PriorityList
{
public:

	PriorityList();

	~PriorityList();

//	is list empty
	bool empty() const;

//	removes and returns first thread according to priority
	Thread* pop();

//	push thread to back of list according to priority
	void push(Thread* thread);

//	remove thread according to id
	void remove(int tid);

//	return number of threads in list
	int size() const;

//	returns true if list contains respective thread
	bool contains(int tid);

private:

	std::list<Thread*> _red;
	std::list<Thread*> _orange;
	std::list<Thread*> _green;
	std::map<int, threadData> _idMap;
};




#endif /* READY_H_ */
