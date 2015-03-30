/*
 * general.h
 *
 *  Created on: Mar 30, 2015
 *      Author: moshemandel
 */

#ifndef GENERAL_H_
#define GENERAL_H_

#define MAX_THREAD_NUM 100 /* maximal number of threads */
#define STACK_SIZE 4096 /* stack size per thread (in bytes) */
#define SUCCESS 0
#define FAILURE -1

typedef enum State { READY, RUNNING, BLOCKED } State;


#endif /* GENERAL_H_ */
