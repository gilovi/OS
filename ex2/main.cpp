/*
 * main.cpp
 *
 *  Created on: Mar 29, 2015
 *      Author: moshemandel
 */

#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <map>
#include "uthreads.h"
#include "thread.h"
#include "priorityList.h"

#define SECOND 1000000

using namespace std;

int totalQuantums = 0;
int suspendedThread = -1;

void foo()
{
	while(true)
	{
		if (uthread_get_total_quantums() > totalQuantums)
		{
			cout<< "thread #: " << uthread_get_tid()  << ". foo: "<< uthread_get_quantums(uthread_get_tid())<<endl;
			totalQuantums = uthread_get_total_quantums();
		}

	}
}

void goo()
{
	while(true)
	{
		if (uthread_get_total_quantums() > totalQuantums)
		{
			cout<< "thread #: " << uthread_get_tid()  << ". goo: "<< uthread_get_quantums(uthread_get_tid() )<<endl;
			totalQuantums = uthread_get_total_quantums();
		}
	}

}

void suspend()
{
	while(true)
	{
		if (uthread_get_total_quantums() > totalQuantums)
		{
			cout<< "thread #: " << uthread_get_tid()  << ". suspend: "<< uthread_get_quantums(uthread_get_tid() )<<endl;
			totalQuantums = uthread_get_total_quantums();
			if(uthread_get_quantums(uthread_get_tid() )%10 == 1 ) {
				cout << "trying to suspend 1"<< endl;
				cout << "return value of suspend: "<<uthread_suspend(1) << endl;
			}
			if (uthread_get_quantums(uthread_get_tid() )%10 == 6 ) {
				cout << "trying to resume 1"<< endl;
				cout << "return value of resume: "<<uthread_resume(1)<< endl;
			}
			totalQuantums = uthread_get_total_quantums();
		}
	}
}

void selfSuspend()
{
	while(true)
	{
		if (uthread_get_total_quantums() > totalQuantums)
		{
			cout<< "thread #: " << uthread_get_tid()  << ". selfSuspend: "<< uthread_get_quantums(uthread_get_tid() )<<endl;
			totalQuantums = uthread_get_total_quantums();
			if(uthread_get_quantums(uthread_get_tid() )%5 == 2 ) {
				cout << "trying to suspend self"<< endl;
				int selfTid = uthread_get_tid();
				suspendedThread = selfTid;
				int retVal = uthread_suspend(selfTid);
				if (retVal == FAILURE)
				{
					suspendedThread = -1;
					cout << "return value of suspend: "<< retVal<< endl;
				}

			}
			totalQuantums = uthread_get_total_quantums();
		}
	}
}

void resume()
{
	while(true)
	{
		if (uthread_get_total_quantums() > totalQuantums)
		{
			cout<< "thread #: " << uthread_get_tid()  << ". resume: "<< uthread_get_quantums(uthread_get_tid() )<<endl;
			if((suspendedThread != -1) && (uthread_get_quantums(uthread_get_tid() )%5 == 0 ))
			{
				cout << "trying to resume " << suspendedThread << endl;
				int retVal = uthread_resume(suspendedThread);
				cout << "return value of resume: "<< retVal << endl;
				if (retVal == SUCCESS)
				{
					suspendedThread = -1;
				}
			}
			totalQuantums = uthread_get_total_quantums();
		}
	}
}

void testList()
{
	PriorityList list;
	Thread* orange0 = new Thread(0, ORANGE);
	Thread* red1 = new Thread(1, RED);
	Thread* green2 = new Thread(2, GREEN);
	Thread* orange3 = new Thread(3, ORANGE);
	Thread* red4 = new Thread(4, RED);
	Thread* green5 = new Thread(5, GREEN);
	cout<<"size: "<<list.size()<<endl;
	cout<<"push orange0."<<endl;
	list.push(orange0);
	cout<<"size: "<<list.size()<<endl;
	cout<<"push red1."<<endl;
	list.push(red1);
	cout<<"size: "<<list.size()<<endl;
	cout<<"push green2."<<endl;
	list.push(green2);
	cout<<"size: "<<list.size()<<endl;
	cout<<"push orange 3."<<endl;
	list.push(orange3);
	cout<<"size: "<<list.size()<<endl;
	cout<<"pop"<<endl;
	list.pop();
	cout<<"size: "<<list.size()<<endl;
	cout<<"remove orange3: " << endl;
	list.remove(3);
	cout<<"size: "<<list.size()<<endl;
	cout<<"remove orange0 + green2" <<endl;
	list.remove(1);
	list.remove(2);
	cout<<"size: "<<list.size()<<endl;
	cout<<"is empty? " << list.empty()<<endl;
}
/*
 *
 * list of tests:
 * minimum available tid chosen?
 * terminate?
 * suspend?
 * resume?
 * getTid?
 * getTotalQuantums?
 * getQuantums?
 * dealing with small quantums (look at question in forum)
*/

// basic switch test
void test1()
{
	cout << "[tester] Test 1: basic test" <<endl;
	if (uthread_init(SECOND) != 0) {
		cout << "[tester] ohh snap! init returned an error code" <<endl;
	}
	else {
		cout <<"[tester] spawn should not return -1: " <<uthread_spawn(foo,RED)<<endl;
		cout <<"[tester] spawn should not return -1: " <<uthread_spawn(goo,RED)<<endl;
	}
}

void test2()
{
	cout << "[tester] Test 2: suspend test and resume" <<endl;
	uthread_init(SECOND);
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(foo,RED) <<endl;
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(suspend,RED) <<endl;
}

void test3()
{
	cout << "[tester] Test 3: selfSuspend test and resume" <<endl;
	uthread_init(SECOND);
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(selfSuspend,RED) <<endl;
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(resume,RED) <<endl;
}

int main()
{
//	std::map<char, int> myMap;
//	myMap['a'] = 1;
//	int count = myMap.erase('b');
//	cout<<count<<endl;
//	count = myMap.erase('a');
//	cout<<count<<endl;
//	testList();

	std::cout<<"test"<<std::endl;
//	test1();
//	test2();
	test3();
	while(true)
	{
//		usleep(SECOND);
		if (uthread_get_total_quantums() > totalQuantums)
		{
			cout<< "thread #: " << uthread_get_tid()  << ". main: "<<uthread_get_quantums(uthread_get_tid()) <<endl;
			totalQuantums = uthread_get_total_quantums();
		}

//		cout<<"main suspending goo"<<endl;
//		uthread_suspend(2);
	}

	return 0;
}



