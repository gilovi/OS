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

void t2()
{
	assert(uthread_get_tid() == 2);
	assert(uthread_get_quantums(2) == 1);
	assert(uthread_get_total_quantums() == 3);

	uthread_suspend(1);

	while (uthread_get_total_quantums() != 5)
		;

	assert(uthread_get_total_quantums() == 5);
	assert(uthread_get_quantums(2) == 2);
	while (true)
	{
		assert(uthread_get_total_quantums() == 5);
	}
}

void t1()
{
	assert(uthread_get_tid() == 1);
	assert(uthread_get_quantums(1) == 1);
	assert(uthread_get_total_quantums() == 2);

	uthread_spawn(t2, Priority::RED);

	while (uthread_get_total_quantums() != 6)
		;
	assert(uthread_get_quantums(0) == 2);
	assert(uthread_get_total_quantums() == 6);

	uthread_terminate(2);

	while (true)
	{
		assert(uthread_get_total_quantums() == 6);
	}

}

void m()
{
	assert(uthread_get_tid() == 0);
	assert(uthread_get_quantums(0) == 1);
	assert(uthread_get_total_quantums() == 1);

	uthread_spawn(t1, Priority::ORANGE);

	while (uthread_get_total_quantums() != 4)
		;
	assert(uthread_get_quantums(0) == 2);
	assert(uthread_get_total_quantums() == 4);

	uthread_resume(1);

	while (uthread_get_total_quantums() != 7)
		;
	assert(uthread_get_quantums(0) == 3);
	uthread_terminate(1);

	while (uthread_get_total_quantums() != 8)
		;
	assert(uthread_get_quantums(0) == 4);
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


