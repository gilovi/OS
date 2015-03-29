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
	if (!_red.empty() )
	{
		Thread* next = _red.front();
		_red.pop();
		return next;
	}
	if (!_orange.empty() )
	{
		Thread* next = _orange.front();
		_orange.pop();
		return next;
	}
	if (!_green.empty() )
	{
		Thread* next = _green.front();
		_green.pop();
		return next;
	}
	return nullptr;
}

void PriorityList::push(Thread* val)
{
	switch (val->getPriority() )
	{
	case RED :
		_red.push(val);
		break;
	case ORANGE :
		_orange.push(val);
		break;
	case GREEN :
		_green.push(val);
		break;
	default :
		return;
	}
}
