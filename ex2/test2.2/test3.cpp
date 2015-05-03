/*
 * main.cpp
 *
 *  Created on: Mar 17, 20015
 *      Author: danny
 */

#include "uthreads.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

void assertTime(int tid, int tquantom, int time)
{
	assert(uthread_get_tid() == tid);
	assert(uthread_get_quantums(tid) == tquantom);
	assert(uthread_get_total_quantums() == time);
}

void loopTill(int time)
{

	while (uthread_get_total_quantums() != time)
		;
}

void t1()
{
	if (uthread_get_total_quantums() == 2)
	{
		assertTime(1, 1, 2);
	}

	if (uthread_get_total_quantums() == 3)
	{
		assertTime(2, 1, 3);
	}

	assert(uthread_suspend(0) == -1);

	while (true)
		;
}

void m()
{
	assertTime(0, 1, 1);

	assert(uthread_spawn(t1, Priority::ORANGE) == 1);
	assert(uthread_spawn(t1, Priority::ORANGE) == 2);

	loopTill(4);

	uthread_terminate(0);
}

int main()
{
	int res = uthread_init(100000);
	if (res == -1)
	{
		return -1;
	}

	m();

	return 0;

}

