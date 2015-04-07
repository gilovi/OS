/*
 * general.h
 *
 *  Created on: Mar 30, 2015
 *      Author: moshemandel
 */

#ifndef GENERAL_H_
#define GENERAL_H_

#include "uthreads.h"
#include <errno.h>
#include <string.h>
#include <iostream>

#define SUCCESS 0
#define FAILURE -1

#define SECOND 1000000

#define INIT_ERR "thread library error: init function -- non-positive quantum length value."
#define SPAWN_ERR "thread library error: spawn function -- number of concurrent is exceeded."
#define TERMINATE_ERR "thread library error: terminate function -- thread does not exist."
#define BLOCK_ERR_MAIN "thread library error: suspend function -- attempted to block main thread."
#define BLOCK_ERR "thread library error: suspend function."
#define RESUME_ERR "thread library error: resume function -- thread does not exist."
#define GET_QUANTUMS_ERR "thread library error: get_quantums function -- thread does not exist."

#define SIGNAL_ERR "system error: signal function -- "
#define SIGEMPTY_ERR "system error: sigemptyset function."
#define SIGADD_ERR	"system error: sigaddset function."
#define SIGPROCMASK_ERR "system error: sigprocmask function -- "
#define SETITIMER_ERR "system error: setitimer function -- "

typedef enum State { READY, RUNNING, BLOCKED, TERMINATED} State;


#endif /* GENERAL_H_ */
