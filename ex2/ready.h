/*
 * ready.h
 *
 *  Created on: Mar 29, 2015
 *      Author: moshemandel
 */

#ifndef READY_H_
#define READY_H_

#include "thread.h"

class Ready{

	Ready();

	~Ready();

	bool empty() const;

	Thread* pop();

	void push();

};




#endif /* READY_H_ */
