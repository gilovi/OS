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

#define SECOND (10^6)

using namespace std;



void foo()
{
	timeval t1, t2, res;
	t1 = {0}, t2 = {0}, res = {0};
	int runningTime = 0;
	gettimeofday(&t1, NULL);
	while(true)
	{
		gettimeofday(&t2, NULL);
		timersub(&t1,&t2,&res);
		runningTime= res.tv_sec*SECOND + res.tv_usec;
		if(runningTime > SECOND)
		{
			cout<<"foo resuming goo"<<endl;
			gettimeofday(&t1, NULL);
			uthread_resume(2);
		}
	}
}

void goo()
{
	while(true)
	{
		usleep(SECOND*0.5);
		cout<<"goo"<<endl;
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
/**
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
int main()
{
//	std::map<char, int> myMap;
//	myMap['a'] = 1;
//	int count = myMap.erase('b');
//	cout<<count<<endl;
//	count = myMap.erase('a');
//	cout<<count<<endl;
	timeval t1, t2, res;
	t1 = {0}, t2 = {0}, res = {0};
	int runningTime = 0;
	std::cout<<"test"<<std::endl;
	uthread_init(1.5*(10^6));
	uthread_spawn(foo, RED);
	uthread_spawn(goo,RED);
	gettimeofday(&t1, NULL);
	while(true)
	{
		gettimeofday(&t2, NULL);
		timersub(&t1,&t2,&res);
		runningTime= res.tv_sec*SECOND + res.tv_usec;
		if(runningTime > SECOND)
		{
			cout<<"main suspending goo"<<endl;
			gettimeofday(&t1, NULL);
			uthread_suspend(2);
		}
	}
//	for(int i = 0; i < 100; i++)
//	{
//		this_thread::sleep_for(chrono::milliseconds(2*(10^3)));
//		timersub(&t2,&t1,&res);
//		runningTime= res.tv_sec*(10^6) + res.tv_usec;
//		std::cout<<"interval time: " << runningTime <<std::endl;
//	}
	return 0;
}


