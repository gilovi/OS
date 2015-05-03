/*
 * test5.cpp
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

void f (void)
{
	int i = 1;
	while(1)
	{
		if (i == uthread_get_quantums(uthread_get_tid()))
		{
			cout << "red1" << endl;
			i++;
		}
		if (i == 4)
		{
			cout << "     red1 suspend" << endl;
			uthread_suspend(1);
		}
		if (i == 8)
		{
			cout << "     exit" << endl;
			uthread_terminate(0);
		}
	}
}

void g (void)
{
	int i = 1;
	while(1)
	{
		if (i == uthread_get_quantums(uthread_get_tid()))
		{
			cout << "red2" << endl;
			i++;
		}
		if (i == 8)
		{
			cout << "     red2 suspend" << endl;
			uthread_suspend(2);
		}
	}
}

void h (void)
{
	int i = 1;
	while(1)
	{
		if (i == uthread_get_quantums(uthread_get_tid()))
		{
			cout << "orange" << endl;
			i++;
		}
		if (i == 12)
		{
			cout << "     orange suspend" << endl;
			uthread_suspend(3);
		}
	}
}

void i (void)
{
	int i = 1;
	while(1)
	{
		if (i == uthread_get_quantums(uthread_get_tid()))
		{
			cout << "green1" << endl;
			i++;
		}
		if (i == 4)
		{
			cout << "     green1 suspend" << endl;
			uthread_suspend(4);
		}
	}
}

void j (void)
{
	int i = 1;
	while(1)
	{
		if (i == uthread_get_quantums(uthread_get_tid()))
		{
			cout << "green2" << endl;
			i++;
		}
		if (i == 8)
		{
			cout << "     green2 suspend" << endl;
			uthread_suspend(5);
		}
	}
}

int main(void)
{
	if (uthread_init(10) == -1)
	{
		return 0;
	}

	uthread_spawn(f,RED);
	uthread_spawn(g,RED);
	uthread_spawn(h,ORANGE);
	uthread_spawn(i,GREEN);
	uthread_spawn(j,GREEN);


	int i = 1;
	int j = 0;


	while(1)
	{
		if (i == uthread_get_quantums(uthread_get_tid()))
		{
			cout << "main" <<  endl;
			i++;
		}
		if (i == 30 && j == 0)
		{
			cout << "     resume green2" << endl;
			uthread_resume(5);
			j++;
		}
		if (i == 34 && j == 1)
		{
			cout << "     resume green1" << endl;
			uthread_resume(4);
			j++;
		}
		if (i == 38 && j == 2)
		{
			cout << "     resume orange" << endl;
			uthread_resume(3);
			j++;
		}
		if (i == 41 && j == 3)
		{
			cout << "     resume red2" << endl;
			uthread_resume(2);
			j++;
		}
		if (i == 44 && j == 4)
		{
			cout << "     resume red1" << endl;
			uthread_resume(1);
			j++;
		}
	}

	uthread_terminate(0);
	return 0;
}







