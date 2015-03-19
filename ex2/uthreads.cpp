/*
 * uthreads.cpp
 *
 *  Created on: Mar 19, 2015
 *      Author: moshemandel
 */

#include "uthreads.h"
#include <queue>

int gTotalQuantums;
Thread gThreads[MAX_THREAD_NUM];
Thread* gRunning;
Thread* gBlocked[MAX_THREAD_NUM];
std::queue<Thread*> gRed;
std::queue<Thread*> gOrange;
std::queue<Thread*> gGreen;
