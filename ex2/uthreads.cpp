/*
 * uthreads.cpp
 *
 *  Created on: Mar 19, 2015
 *      Author: moshemandel
 */
#include "uthreads.h"

#include <queue>
#include <signal.h>
#include <sys/time.h>
#include <cmath>
#include<stddef.h>
#include "thread.h"
#include "scheduler.h"
#include "priorityList.h"

//TODO: remove
#include <iostream>

int gTotalQuantums;
int gQuantum_usecs;
Thread* gThreads[MAX_THREAD_NUM];
Thread* gRunning;
Thread* gBlocked[MAX_THREAD_NUM];
PriorityList gReady;
std::priority_queue<int, std::vector<int>, std::greater<int> > availableID;


void scheduler(int sig)
{
//    std::cout<<gRunning->getID()<<std::endl;
//    std::cin.get();
    if (!gReady.empty())
    {
        Thread *next = gReady.pop();
        gReady.push(gRunning);

        if (!sigsetjmp(*(gRunning->getThreadState() ),1))
        {
            gRunning = next;
            gRunning->increaseQuantum();
            siglongjmp(*(gRunning->getThreadState() ),1);
        }
    }
    else
    {
        gRunning->increaseQuantum();
    }

}

void initTimer()
{
    signal(SIGVTALRM, scheduler);

    struct itimerval tv;
    int seconds = floor(gQuantum_usecs*(10^(-6)) );
    int microseconds = gQuantum_usecs - seconds*(10^6);
    tv.it_value.tv_sec = 0;  /* first time interval, seconds part */
    tv.it_value.tv_usec = 1000; /* first time interval, microseconds part */
    tv.it_interval.tv_sec = 0;  /* following time intervals, seconds part */
    tv.it_interval.tv_usec = 1000; /* following time intervals, microseconds part */

    setitimer(ITIMER_VIRTUAL, &tv, NULL);

    gRunning = gReady.pop();
    siglongjmp(*(gRunning->getThreadState() ),1);

}

int uthread_init(int quantum_usecs)
{
    for (int i = 1; i< MAX_THREAD_NUM; i++)
    {
        availableID.push(i);
    }
	gQuantum_usecs = quantum_usecs;
	Thread *mainThread = new Thread(0, ORANGE);

    if (!sigsetjmp(*(mainThread->getThreadState() ),1))
    {
        gReady.push(mainThread);
        initTimer();
    }


//	TODO: what is considered a failure? -- when do we return (-1)?
	return SUCCESS;
}

int uthread_spawn(void (*f)(void), Priority pr)
{
    Thread *newThread = new Thread(availableID.top(), f, pr);
    availableID.pop();
    gReady.push(newThread);

    return SUCCESS;
}

#ifdef __x86_64__
/* code for 64 bit Intel arch */

/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%fs:0x30,%0\n"
		"rol    $0x11,%0\n"
                 : "=g" (ret)
                 : "0" (addr));
    return ret;
}

#else
/* code for 32 bit Intel arch */

/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%gs:0x18,%0\n"
		"rol    $0x9,%0\n"
                 : "=g" (ret)
                 : "0" (addr));
    return ret;
}




#endif
