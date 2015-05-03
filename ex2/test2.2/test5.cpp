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
	loopTill(13);
	assert(uthread_resume(1) == 0);
	loopTill(16);
	uthread_terminate(5);

}

void t4()
{
	assertTime(4, 1, 20);
	uthread_terminate(4);

}
void t3()
{
	assertTime(3, 1, 5);

	assert(uthread_spawn(t4, Priority::GREEN) == 4);
	assert(uthread_spawn(t5, Priority::ORANGE) == 5);
	loopTill(7);

	assertTime(3, 2, 7);
	loopTill(9);
	loopTill(11);
	uthread_terminate(3);
}

void t2()
{
	assertTime(2, 1, 18);
	loopTill(22);
	assertTime(2, 2, 22);
while(true);
}

void t1()
{
	assertTime(1, 1, 2);

	loopTill(4);
	assertTime(1, 2, 4);
	assert(uthread_spawn(t3, Priority::RED) == 3);
	loopTill(6);

	assert(uthread_suspend(1) == 0);
	uthread_terminate(1);
}

void m()
{
	assertTime(0, 1, 1);

	assert(uthread_spawn(t1, Priority::RED) == 1);
	assert(uthread_spawn(t2, Priority::GREEN) == 2);
	loopTill(8);
	loopTill(12);
	loopTill(15);
	loopTill(17);
	loopTill(19);
	loopTill(21);
	loopTill(23);
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

