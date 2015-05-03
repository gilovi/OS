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
	assertTime(1, 1, 5);
	loopTill(7);
	assertTime(1, 2, 7);
	loopTill(9);
	assertTime(1, 3, 9);
	uthread_terminate(1);
}

void m()
{
	assertTime(0, 1, 1);
	assert(uthread_get_quantums(55) == -1);
	loopTill(2);
	assertTime(0, 2, 2);
	loopTill(3);
	assertTime(0, 3, 3);
	loopTill(4);
	assertTime(0, 4, 4);
	assert(uthread_spawn(t1, Priority::RED) == 1);
	loopTill(6);
	assertTime(0, 5, 6);
	loopTill(8);
	assertTime(0, 6, 8);
	loopTill(10);
	assertTime(0, 7, 10);
	loopTill(11);
	assertTime(0, 8, 11);
	uthread_terminate(0);
}

int main()
{
	int res = uthread_init(1);
	if (res == -1)
	{
		return -1;
	}

	m();

	return 0;

}

