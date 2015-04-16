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

void t5()
{
}

void t4()
{
	assertTime(4, 1, 7);
	assert(uthread_suspend(5) == 0); // suspened

	while (true)
		;

}
void t3()
{
	assertTime(3, 1, 4);

	assert(uthread_spawn(t4, Priority::ORANGE) == 4);
	assert(uthread_spawn(t5, Priority::ORANGE) == 5);

	assert(uthread_suspend(2) == 0); // resume suspended

	loopTill(8);

	assertTime(3, 2, 8);

	assert(uthread_suspend(4) == 0); // suspened

	loopTill(12);

	assertTime(3, 3, 12);
	uthread_terminate(0);
}

void t2()
{
	assertTime(2, 1, 3);

	assert(uthread_suspend(2) == 0); // suspend self

	assertTime(2, 2, 9);
	assert(uthread_suspend(5) == 0); // suspend suspended

	while (true)
		;
}

void t1()
{
	assertTime(1, 1, 2);

	assert(uthread_resume(1) == 0); // resume running
	assert(uthread_resume(0) == 0); // resume ready

	loopTill(6);
	assertTime(1, 2, 6);

	loopTill(11);

	uthread_terminate(5); // terminate dead thread
	uthread_suspend(5); // suspend dead thread
	uthread_resume(5); // resume dead thread

	uthread_terminate(1);

	assert(false);
}

void m()
{
	assertTime(0, 1, 1);

	assert(uthread_spawn(t1, Priority::ORANGE) == 1);
	assert(uthread_spawn(t2, Priority::ORANGE) == 2);
	assert(uthread_spawn(t3, Priority::ORANGE) == 3);

	loopTill(5);

	assertTime(0, 2, 5);
	assert(uthread_resume(2) == 0); // resume blocked

	loopTill(10);

	assertTime(0, 3, 10);

	assert(uthread_terminate(5) == 0); // terminated suspened

	while (true)
		;
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

