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
#include <stdlib.h>
#include<stddef.h>
#include <map>
#include "thread.h"
#include "scheduler.h"
#include "priorityList.h"

//TODO: remove
#include <iostream>
//timeval t1, t2, res;
//int runningTime = 0;

int gTotalQuantums;
int gQuantum_usecs;
//TODO: is it necessary to hold a container for all thread pointers? or is it enough to hold gReady,gRunning & gBlocked?
//		consider what to do when deleting a thread...
//Thread* gThreads[MAX_THREAD_NUM];
std::map<int, Thread*> gThreads;
Thread* gRunning;
//Thread* gBlocked[MAX_THREAD_NUM];
std::map<int, Thread*> gBlocked;
PriorityList gReady;
std::priority_queue<int, std::vector<int>, std::greater<int> > availableID;


void scheduler(int sig)
{
//TODO: remove
//	if (timercmp(&t1,&t2,<) )
//	{
//		gettimeofday(&t1, NULL);
//		timersub(&t1,&t2,&res);
//	}
//	else
//	{
//		gettimeofday(&t2,NULL);
//		timersub(&t2,&t1,&res);
//	}
//	runningTime= res.tv_sec*(10^6) + res.tv_usec;
//	std::cout<<"interval time: " << runningTime <<std::endl;
//    std::cout<<gRunning->getID() << "--> " << sig <<std::endl;
//    std::cin.get();

	gTotalQuantums++;
    if (!gReady.empty())
    {
        Thread *next = gReady.pop();
        gReady.push(gRunning);
        gRunning->setState(READY);
        if (!sigsetjmp(*(gRunning->getThreadState() ),1))
        {
            gRunning = next;
            next->setState(RUNNING);
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

//    TODO: remove
//    t1 = {0}, t2 = {0}, res = {0};
//    gettimeofday(&t1,NULL);

    struct itimerval tv;
    int seconds = floor(gQuantum_usecs/(10^6) );
    int microseconds = gQuantum_usecs - seconds*(10^6);

//    TODO: remove
//    std::cout<<"seconds: " << seconds << "\nmicorseconds: " << microseconds << std::endl;
//    std::cin.get();

    tv.it_value.tv_sec = seconds;  /* first time interval, seconds part */
    tv.it_value.tv_usec = microseconds; /* first time interval, microseconds part */
    tv.it_interval.tv_sec = seconds;  /* following time intervals, seconds part */
    tv.it_interval.tv_usec = microseconds; /* following time intervals, microseconds part */

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
	gTotalQuantums = 1;
	Thread *mainThread = new Thread(0, ORANGE);
	gThreads[0] = mainThread;
	mainThread->increaseQuantum();
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
	int tid = availableID.top();
	if (tid < MAX_THREAD_NUM)
	{
		Thread *newThread = new Thread(tid, f, pr);
		gThreads[tid] = newThread;
		availableID.pop();
		gReady.push(newThread);
		return tid;
	}
    return FAILURE;
}

int uthread_terminate(int tid)
{
	if(tid == 0)//if true: exits program
	{
//		terminate all threads:
		std::map<int, Thread*>::iterator threadIt;
		for (threadIt = gThreads.begin(); threadIt != gThreads.end(); ++threadIt)
		{
			switch(threadIt->second->getState() )
			{
			case(READY) :
					gReady.remove(threadIt->second->getID() );
					break;
			case(RUNNING) :
					gRunning = NULL;
					break;
			case(BLOCKED) :
					gBlocked.erase(threadIt->second->getID() );
					break;
			default :
				break;
			}
			Thread* tmp = gThreads[threadIt->second->getID() ];
			gThreads.erase(threadIt);
			delete tmp;
		}
		exit(0);
	}
	if(!gThreads.count(tid) ) //if true: thread doesn't exist
	{
		return FAILURE;
	}
	if(gThreads[tid]->getState() == RUNNING)//if true: deletes thread +  jumps to next thread
	{
		gRunning = gReady.pop();
		gRunning->setState(RUNNING);
		Thread* tmp = gThreads[tid];
		gThreads.erase(tid);
		delete tmp;
		siglongjmp(*(gRunning->getThreadState() ),1);
	}
//	if in ready or blocked: remove from lists (gReady/gBlocked + gThreads), and delete thread
	Thread* tmp = gThreads[tid];
	if (tmp->getState() == READY )
	{
		gReady.remove(tid);
	}
	if (tmp->getState() == BLOCKED)
	{
		gBlocked.erase(tid);
	}
	gThreads.erase(tid);
	delete tmp;
	return SUCCESS;
}

int uthread_suspend(int tid)
{
	if(tid == 0)
	{
		return FAILURE;
	}
	Thread* tmp = gThreads[tid];
	if(tmp->getState() == BLOCKED )
	{
//		no effect
		return SUCCESS;
	}
	if(tmp->getState() == RUNNING )
	{
//		make scheduling decision (move to next thread)
		gBlocked[tid] = gRunning;
		tmp->setState(BLOCKED);
		gRunning = gReady.pop();
		gRunning->setState(RUNNING);
		siglongjmp(*(gRunning->getThreadState() ),1);
	}
	if(tmp->getState() == READY )
	{
//		move to gBlocked + update state in thread
		gReady.remove(tid);
		gBlocked[tid] = tmp;
		tmp->setState(BLOCKED);
		return SUCCESS;
	}
	return FAILURE;
}

int uthread_resume(int tid)
{
	if(!gThreads.count(tid) )
	{
		return FAILURE;
	}
	Thread* tmp = gThreads[tid];
	if (tmp->getState() == READY || tmp->getState() == RUNNING ) //no effect
	{
		return SUCCESS;
	}
//	otherwise the thread should be in gBlocked (assuming no bugs...)
	gBlocked.erase(tid);
	gReady.push(tmp);
	tmp->setState(READY);
	return SUCCESS;
}

int uthread_get_tid()
{
	return (*gRunning).getID();
}

int uthread_get_total_quantums()
{
	return gTotalQuantums;
}

int uthread_get_quantums(int tid)
{
	return gThreads[tid]->getQuantums();
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
