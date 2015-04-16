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

	while (true)
		;
}

void m()
{
	assertTime(0, 1, 1);

	for (int i = 1; i < 100; i++)
	{
		assert(uthread_spawn(t1, Priority::ORANGE) == i);
	}

	for (int i = 20; i < 65; i += 5)
	{
		assert(uthread_terminate(i) == 0);
	}

	for (int i = 0; i < 9; i++)
	{
		assert(uthread_spawn(t1, Priority::ORANGE) == 20 + i * 5);
	}

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

