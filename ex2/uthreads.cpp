/*
 * uthreads.cpp
 *
 *  Created on: Mar 19, 2015
 *      Author: moshemandel
 */
#include "uthreads.h"
#include "general.h"

#include <queue>
#include <signal.h>
#include <sys/time.h>
#include <cmath>
#include <map>
#include "thread.h"
#include "priorityList.h"

int gTotalQuantums;
int gQuantum_usecs;
struct sigaction gAct;
sigset_t gSignalSet;

std::map<int, Thread*> gThreads;
Thread* gRunning;
std::map<int, Thread*> gBlocked;
PriorityList gReady;
std::priority_queue<int, std::vector<int>, std::greater<int> > gAvailableID;

struct itimerval gTimer;

void blockTimer()
{
	if(sigprocmask(SIG_BLOCK,&gSignalSet,NULL) == FAILURE)
	{
		int errTmp = errno;
		std::cerr<< SIGPROCMASK_ERR << strerror(errTmp) << " -- tried to block" <<std::endl;
		exit(1);
	}
}

void resumeTimer()
{
	if (sigprocmask(SIG_UNBLOCK,&gSignalSet,NULL) == FAILURE)
	{
		int errTmp = errno;
		std::cerr<< SIGPROCMASK_ERR << strerror(errTmp) << " -- tried to unblock" << std::endl;
		exit(1);
	}
}

//state determines to which list/state gRunning will be pushed to.
void switchThreads(State state)
{
	gTotalQuantums++;
	if(gReady.empty() )
//		either main thread is the only thread and it is running.
//		or other thread is running and main must be in ready list.
	{
		gRunning->increaseQuantum();
	}
	else
	{
		Thread* next = gReady.pop();
		int retVal = 0;
		switch(state)
		{
		case READY :
			gReady.push(gRunning);
			gRunning->setState(READY);
			retVal = sigsetjmp(*(gRunning->getThreadState() ),1);
			break;
		case BLOCKED :
			gRunning->setState(BLOCKED);
			gBlocked[gRunning->getID()] = gRunning;
			retVal = sigsetjmp(*(gRunning->getThreadState() ),1);
			break;
		case TERMINATED :
			gThreads.erase(gRunning->getID() );
			gAvailableID.push(gRunning->getID() );
			delete gRunning;
			retVal = 0;
			break;
		default :
			break;
		}
		if (retVal == 0)
		{
			gRunning = next;
			next->setState(RUNNING);
			gRunning->increaseQuantum();
			siglongjmp(*(gRunning->getThreadState() ),1);
		}
	}

	if (setitimer(ITIMER_VIRTUAL, &gTimer, NULL) == FAILURE)
	{
		int errTmp = errno;
		std::cerr<< SETITIMER_ERR << strerror(errTmp) << std::endl;
		exit(1);

	}
}

void timerHandler(int sig)
{
	switchThreads(READY);
}

void initTimer()
{
    int seconds = floor(gQuantum_usecs/SECOND );
    int microseconds = gQuantum_usecs - seconds*SECOND;

    gTimer.it_value.tv_sec = seconds;  /* first time interval, seconds part */
    gTimer.it_value.tv_usec = microseconds; /* first time interval, microseconds part */
    gTimer.it_interval.tv_sec = seconds;  /* following time intervals, seconds part */
    gTimer.it_interval.tv_usec = microseconds; /* following time intervals, microseconds part */
}

int uthread_init(int quantum_usecs)
{
	if (quantum_usecs <= 0)
	{
		std::cerr << INIT_ERR << std::endl;
		return FAILURE;
	}
    for (int i = 1; i< MAX_THREAD_NUM +1; i++)
    {
        gAvailableID.push(i);
    }
	gQuantum_usecs = quantum_usecs;
	gTotalQuantums = 0;
	Thread *mainThread = new Thread(0, ORANGE);
	gThreads[0] = mainThread;

//	initialize signal handler
	gAct.sa_handler = timerHandler;
	if (sigemptyset(&gAct.sa_mask)== FAILURE)
	{
		std::cerr << SIGEMPTY_ERR << strerror(EINVAL) << std::endl;
		exit(1);
	}
	gAct.sa_flags = 0;
	if (sigaction(SIGVTALRM, &gAct, NULL) == FAILURE)
	{
		std::cerr<< SIGACTION_ERR <<std::endl;
		exit(1);
	}

	initTimer();
	gRunning = mainThread;
	mainThread->setState(RUNNING);
	switchThreads(RUNNING);

	// create sigset for blocking signals later on
	if (sigemptyset(&gSignalSet) == FAILURE)
	{
		std::cerr << SIGEMPTY_ERR << strerror(EINVAL) << std::endl;
		exit(1);
	}
	if (sigaddset(&gSignalSet,SIGVTALRM) == FAILURE)
	{
		std::cerr << SIGADD_ERR << strerror(EINVAL) << std::endl;
		exit(1);
	}

	return SUCCESS;
}

int uthread_spawn(void (*f)(void), Priority pr)
{
	blockTimer();
	int tid = gAvailableID.top();
	if (tid < MAX_THREAD_NUM)
	{
		Thread *newThread = new Thread(tid, f, pr);
		gThreads[tid] = newThread;
		gAvailableID.pop();
		gReady.push(newThread);
		resumeTimer();
		return tid;
	}
	std::cerr<< SPAWN_ERR << std::endl;
	resumeTimer();
    return FAILURE;
}

int uthread_terminate(int tid)
{
	blockTimer();
	if(tid == 0)//if true: exits program
	{
//		terminate all threads:
		std::map<int, Thread*>::iterator threadIt;
		for (threadIt = gThreads.begin(); threadIt != gThreads.end(); ++threadIt)
		{
			Thread* tmp;
			switch(threadIt->second->getState() )
			{
			case(READY) :
					gReady.remove(threadIt->second->getID() );
					tmp = gThreads[threadIt->second->getID() ];
					gThreads.erase(threadIt);
					delete tmp;
					break;
			case(RUNNING) :
					gThreads.erase(threadIt);
//					gRunning = NULL;
					break;
			case(BLOCKED) :
					gBlocked.erase(threadIt->second->getID() );
					tmp = gThreads[threadIt->second->getID() ];
					gThreads.erase(threadIt);
					delete tmp;
					break;
			default :
				break;
			}

		}
		delete gRunning;
		gRunning = NULL;
		resumeTimer();
		exit(0);
	}
	if(gThreads.count(tid) == 0) //if true: thread doesn't exist
	{
		std::cerr<< TERMINATE_ERR << std::endl;
		resumeTimer();
		return FAILURE;
	}
	if(gThreads[tid]->getState() == RUNNING)//if true: deletes thread +  jumps to next thread
	{
		switchThreads(TERMINATED);
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
	gAvailableID.push(tid);
	delete tmp;
	resumeTimer();
	return SUCCESS;
}

int uthread_suspend(int tid)
{
	blockTimer();
	if(tid == 0)
	{
		std::cerr<< BLOCK_ERR_MAIN <<std::endl;
		resumeTimer();
		return FAILURE;
	}
	if(gThreads.count(tid) == 0)
	{
        std::cerr << BLOCK_ERR << " Thread does not exist." << std::endl;
        resumeTimer();
        return FAILURE;
	}
	Thread* tmp = gThreads[tid];
	if(tmp->getState() == BLOCKED )
	{
//		no effect
		resumeTimer();
		return SUCCESS;
	}
	if(tmp->getState() == RUNNING )
	{
//		make scheduling decision (move to next thread)
		switchThreads(BLOCKED);
		resumeTimer();
		return SUCCESS;
	}
	if(tmp->getState() == READY )
	{
//		move to gBlocked + update state in thread
		gReady.remove(tid);
		gBlocked[tid] = tmp;
		tmp->setState(BLOCKED);
		resumeTimer();
		return SUCCESS;
	}
	std::cerr << BLOCK_ERR << std::endl;
	resumeTimer();
	return FAILURE;
}

int uthread_resume(int tid)
{
	blockTimer();
	if(gThreads.count(tid) == 0)
//		thread does not exist -- error
	{
		std::cerr << RESUME_ERR << std::endl;
		resumeTimer();
		return FAILURE;
	}
	Thread* tmp = gThreads[tid];
	if (tmp->getState() == READY || tmp->getState() == RUNNING ) //no effect
	{
		resumeTimer();
		return SUCCESS;
	}
//	otherwise the thread should be in gBlocked (assuming no bugs...)
	gBlocked.erase(tid);
	gReady.push(tmp);
	tmp->setState(READY);
	resumeTimer();
	return SUCCESS;
}

int uthread_get_tid()
{
	blockTimer();
	int tid = (*gRunning).getID();
	resumeTimer();
	return tid;
}

int uthread_get_total_quantums()
{
	return gTotalQuantums;
}

int uthread_get_quantums(int tid)
{
	blockTimer();
	if(gThreads.count(tid) == 0)
	{
		std::cerr << GET_QUANTUMS_ERR << std::endl;
		resumeTimer();
		return FAILURE;
	}
	resumeTimer();
	return gThreads[tid]->getQuantums();
}
