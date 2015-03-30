/*
 * ready.cpp
 *
 *  Created on: Mar 29, 2015
 *      Author: moshemandel
 */

#include "priorityList.h"

PriorityList::PriorityList()
{

}

PriorityList::~PriorityList()
{

}

bool PriorityList::empty() const
{
	return (_red.empty() && _orange.empty() && _green.empty() );
}

//assumes list is non-empty (if empty: returns nullptr)
Thread* PriorityList::pop()
{
	Thread* next;
	if (!_red.empty() )
	{
		next = _red.front();
		_red.pop_back();
	}
	else if (!_orange.empty() )
	{
		next = _orange.front();
		_orange.pop_back();
	}
	else if (!_green.empty() )
	{
		next = _green.front();
		_green.pop_back();
	}
	else
	{
		return nullptr;
	}
	_idMap.erase(next->getID() );
	return next;
}

void PriorityList::push(Thread* thread)
{
	switch (thread->getPriority() )
	{
	case RED :
		_red.push_front(thread);
//		threadData tmp = {RED, _red.begin()};
		_idMap[thread->getID()] = {RED, _red.begin()};
		break;
	case ORANGE :
		_orange.push_front(thread);
//		threadData tmp = {ORANGE, _orange.begin()};
		_idMap[thread->getID()] = {ORANGE, _orange.begin()};
		break;
	case GREEN :
		_green.push_front(thread);
//		threadData tmp = {GREEN, _green.begin()};
		_idMap[thread->getID()] = {GREEN, _green.begin()};
		break;
	default :
		return;
	}
}

void PriorityList::remove(int tid)
{
	if (_idMap.count(tid) ) // check if thread with id exists in list
	{
		threadData data = _idMap[tid];
		switch(data.priority)
		{
		case RED:
			_red.erase(data.threadIt);
			break;
		case ORANGE:
			_orange.erase(data.threadIt);
			break;
		case GREEN:
			_green.erase(data.threadIt);
			break;
		default:
			return;
		}
		_idMap.erase(tid);
	}
}

int PriorityList::size() const
{
	return _idMap.size();
}

bool PriorityList::contains(int tid)
{
	return _idMap.count(tid);
}
