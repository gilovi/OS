#include "uthreads.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

void assertTime(int tid, int tquantom, int time)
{
	//printf("%d, %d, %d\n", tid, tquantom, time);
	//printf("%d, %d, %d\n", uthread_get_tid(), uthread_get_quantums(tid), uthread_get_total_quantums());
	assert(uthread_get_tid() == tid);
	assert(uthread_get_quantums(tid) == tquantom);
	assert(uthread_get_total_quantums() == time);
}

void loopTill(int time)
{

	while (uthread_get_total_quantums() != time)
		;
}

void t1() {
	static int i = 1;

	while(true) {
		int x = i + i;
		assertTime(1, i, x);
		//printf("t1\n");
		loopTill(x + 2);
		i = i + 1;

		assert(uthread_terminate(2) == 0);
	}

}

void m() {
	static int i = 1;

	assert(uthread_spawn(t1, Priority::ORANGE) == 1);

	while(true) {
		int x = i + i - 1;
		assertTime(0, i, x);
		//printf("t0\n");
		loopTill(x +2);
		i = i + 1;
		if (i > 100)
			uthread_terminate(0);

		assert(uthread_spawn(t1, Priority::GREEN) == 2);
	}
}


int main()
{
	
	int res = uthread_init(1);
	if (res == -1)
	{
		return -1;
	}

	m();

	return 0;

}
