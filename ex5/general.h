/*
 * general.h
 *
 *  Created on: Jun 16, 2015
 *      Author: gilovi
 */

#ifndef GENERAL_H_
#define GENERAL_H_

#include <errno.h>
#include <iostream>

#define SUCCESS 0
#define ERROR -1
#define ERROR_CODE 1
#define BUFF_SIZE 1024
#define MIN_PORT_NUM 1
#define MAX_PORT_NUM 65535

#define sysErr(sysCall, err) "Error: function:" << sysCall << " errno:" << err << "."

#endif /* GENERAL_H_ */
