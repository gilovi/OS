/*
 * test3.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: roigreenberg
 */

#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <deque>
#include <list>
#include <assert.h>
#include "../uthreads.h"
//#include "libuthread.a"
#include <iostream>

using namespace std;

void f(void){}

int main(void)
{
	if (uthread_init(1000000000) == -1)
	{
		return 0;
	}


	uthread_terminate(-1);
	uthread_suspend(-1);
	uthread_resume(-1);
	uthread_get_quantums(-1);

	uthread_terminate(1);
	uthread_suspend(1);
	uthread_resume(1);
	uthread_get_quantums(1);

	uthread_suspend(0);

	uthread_spawn(f,GREEN);
	uthread_terminate(1);

	uthread_terminate(1);
	uthread_suspend(1);
	uthread_resume(1);
	uthread_get_quantums(1);

	uthread_init(0);
	uthread_init(-5);


	uthread_terminate(0);
	return 0;
}

