/*
 * uthreads.cpp
 *
 *  Created on: Mar 19, 2015
 *      Author: moshemandel
 */

#include "uthreads.h"
#include <queue>

int gTotalQuantums;
int gQuantum_usecs;
Thread gThreads[MAX_THREAD_NUM];
Thread* gRunning;
Thread* gBlocked[MAX_THREAD_NUM];
std::queue<Thread*> gRed;
std::queue<Thread*> gOrange;
std::queue<Thread*> gGreen;

int uthread_init(int quantum_usecs)
{
	gQuantum_usecs = quantum_usecs;
	Thread mainThread = new Thread(0, ORANGE);

//	TODO: what is considered a failure? -- when do we return (-1)?
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
