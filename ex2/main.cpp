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

#define QUANTUM_LENGTH 100000
#define RUNS 3
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
		if (uthread_get_quantums(uthread_get_tid()) > RUNS)
		{
            uthread_terminate(uthread_get_tid());
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
		if (uthread_get_quantums(uthread_get_tid()) > RUNS)
		{
            uthread_terminate(uthread_get_tid());
		}

	}

}

void loop()
{
	while(true)
	{
		cout<<"loop"<<endl;
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
		if (uthread_get_quantums(uthread_get_tid()) > RUNS)
		{
            uthread_terminate(uthread_get_tid());
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
		if (uthread_get_quantums(uthread_get_tid()) > RUNS)
		{
            uthread_terminate(uthread_get_tid());
        }
	}
}

void selfTerminate()
{
    while(true)
	{
		if (uthread_get_total_quantums() > totalQuantums)
		{
			cout<< "thread #: " << uthread_get_tid()  << ". selfTerminate: "<< uthread_get_quantums(uthread_get_tid() )<<endl;
			totalQuantums = uthread_get_total_quantums();
			if(uthread_get_quantums(uthread_get_tid() ) == 5 )
            {
				cout << "trying to teminate self"<< endl;
				int selfTid = uthread_get_tid();
				int retVal = uthread_terminate(selfTid);
				if (retVal == FAILURE)
				{
                    cout << "return value of terminate: "<< retVal<< endl;
				}

			}
		}
		if (uthread_get_quantums(uthread_get_tid()) > RUNS)
		{
            uthread_terminate(uthread_get_tid());
		}
    }

}
void whatsMyId()
{
    while (true)
    {
        if (uthread_get_total_quantums() > totalQuantums)
        {
            totalQuantums = uthread_get_total_quantums();
            cout<< "thread #: " << uthread_get_tid()  << ". whatsMyId: "<< uthread_get_quantums(uthread_get_tid() )<<endl;
            cout<< "My Id is: "<<uthread_get_tid() << endl;
        }
        if (uthread_get_quantums(uthread_get_tid()) > RUNS)
		{
            uthread_terminate(uthread_get_tid());
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
		if (uthread_get_quantums(uthread_get_tid()) > RUNS)
		{
            uthread_terminate(uthread_get_tid());
		}
	}
}
void nest2()
{
while(true)
	{
		if (uthread_get_total_quantums() > totalQuantums)
		{
		    totalQuantums = uthread_get_total_quantums();
			cout<< "thread #: " << uthread_get_tid()  << ". nest2: "<< uthread_get_quantums(uthread_get_tid() )<<endl;
			if(uthread_get_quantums(uthread_get_tid() ) == 3 )
            {
                uthread_spawn(foo,RED);
            }

        }
        if (uthread_get_quantums(uthread_get_tid()) > RUNS)
		{
            uthread_terminate(uthread_get_tid());
		}
    }

}

void nest1()
{
while(true)
	{
		if (uthread_get_total_quantums() > totalQuantums)
		{
		    totalQuantums = uthread_get_total_quantums();
			cout<< "thread #: " << uthread_get_tid()  << ". nest1: "<< uthread_get_quantums(uthread_get_tid() )<<endl;
			if(uthread_get_quantums(uthread_get_tid() ) == 3 )
            {
                uthread_spawn(nest2,RED);
            }

        }
        if (uthread_get_quantums(uthread_get_tid()) > RUNS)
		{
            uthread_terminate(uthread_get_tid());
		}
}

}

void terminateMain()
{
    uthread_terminate(0);
    cout<<"If this line is printed, something is wrong!"<<endl;
    uthread_terminate(uthread_get_tid());
}

void suspendMain()
{
    while (true)
    {
        if (uthread_get_total_quantums() > totalQuantums)
        {
            totalQuantums = uthread_get_total_quantums();
            cout<< "thread #: " << uthread_get_tid()  << ". suspendMain: "<< uthread_get_quantums(uthread_get_tid() )<<endl;
            cout<< "suspending main returns: "<<uthread_suspend(0)<< endl;
        }
        if (uthread_get_quantums(uthread_get_tid()) > RUNS)
		{
            uthread_terminate(uthread_get_tid());
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
	if (uthread_init(QUANTUM_LENGTH) != 0) {
		cout << "[tester] ohh snap! init returned an error code" <<endl;
		exit(-1);
	}
	else {
		cout <<"[tester] spawn should not return -1: " <<uthread_spawn(foo,RED)<<endl;
		cout <<"[tester] spawn should not return -1: " <<uthread_spawn(goo,RED)<<endl;
	}
}

void test2()
{
	cout << "[tester] Test 2: suspend test and resume" <<endl;
	uthread_init(QUANTUM_LENGTH);
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(foo,RED) <<endl;
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(suspend,RED) <<endl;
}

void test3()
{
	cout << "[tester] Test 3: selfSuspend test and resume" <<endl;
	uthread_init(QUANTUM_LENGTH);
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(selfSuspend,RED) <<endl;
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(resume,RED) <<endl;
}
void test4()
{
    cout << "[tester] Test 4: self Terminate test" <<endl;
	uthread_init(QUANTUM_LENGTH);
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(selfTerminate,RED) <<endl;

}

void test5()
{
    cout << "[tester] Test 5: enumuration check (id should be 1)" <<endl;
	uthread_init(QUANTUM_LENGTH);
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(foo,RED) <<endl;
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(goo,RED) <<endl;
	uthread_terminate(1);
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(whatsMyId,RED) <<endl;

}
void test6()
{
    cout << "[tester] Test 6: nested spawn" <<endl;
//	uthread_init(QUANTUM_LENGTH);
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(nest1,GREEN) <<endl;
}

void test7()
{
    cout << "[tester] Test 7: resume running thread" << endl;
//    uthread_init(QUANTUM_LENGTH);
    //some other threads:
    int suspend = uthread_spawn(foo,RED);
    cout <<"[tester] spawn should not return -1: " << suspend << endl;
    int terminate = uthread_spawn(foo,GREEN);
    cout <<"[tester] spawn should not return -1: " << terminate << endl;
    int ready = uthread_spawn(goo,ORANGE);
    cout <<"[tester] spawn should not return -1: " <<ready << endl;

    uthread_suspend(suspend);
    uthread_terminate(terminate);

    cout << "resuming main. should not return -1: " << uthread_resume(0) << endl;
}
void test8()
{

    cout << "[tester] Test 8: terminating suspended thread" << endl;
    uthread_init(QUANTUM_LENGTH);
    //some other threads:
    int suspend = uthread_spawn(foo,RED);
    cout << "[tester] spawn should not return -1: " << suspend << endl;
    int terminate = uthread_spawn(foo,GREEN);
    cout << "[tester] spawn should not return -1: " << terminate << endl;
    int ready = uthread_spawn(goo,ORANGE);
    cout << "[tester] spawn should not return -1: " << ready << endl;

    uthread_suspend(suspend);
    uthread_terminate(terminate);

    cout << "double check. trying to resume the terminated:" << uthread_resume(suspend) << endl;
    cout << "terminating. should return 0" << uthread_terminate(suspend) << endl;

}

void test9()
{
	uthread_init(100);
//	int red1 = uthread_spawn(loop,RED);
//	int quantums1 = uthread_get_quantums(1);
//	cout << "quantums1: " << quantums1 << endl;
//	cout<< "suspend return value: " <<suspend <<endl;
}

// error testes//
void err1()
{
cout << "[tester] err1: main termination" <<endl;
	uthread_init(QUANTUM_LENGTH);
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(foo,RED) <<endl;
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(foo,GREEN) <<endl;
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(terminateMain,ORANGE) <<endl;
}
void err2()
{
cout << "[tester] err2: main suspend" <<endl;
	uthread_init(QUANTUM_LENGTH);
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(foo,RED) <<endl;
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(foo,GREEN) <<endl;
	cout <<"[tester] spawn should not return -1: " <<uthread_spawn(suspendMain,ORANGE) <<endl;
}
void err3()
{
    cout << "[tester] err3: Thread Overflow" <<endl;
	uthread_init(QUANTUM_LENGTH);
	for (int i = 1 ; i < MAX_THREAD_NUM ; i++)
    {
        cout << "spawn thread #" << i << endl;
        cout <<"[tester] spawn should not return -1: " <<uthread_spawn(foo,ORANGE) <<endl;
    }
    cout << "spawn thread #" << MAX_THREAD_NUM  << endl;
    int ret = uthread_spawn(foo,ORANGE);
    cout <<"[tester] spawn SHOULD return -1: " << ret <<endl;
   // if (ret == -1) exit(0);

}

void err4()
{
    cout << "[tester] err4: zero time init" <<endl;
	cout << "" << uthread_init(0) <<endl;
}

void err5()
{
    cout << "[tester] err5: wrong id" << endl;
    uthread_init(QUANTUM_LENGTH);
	//some other threads:
    int suspend = uthread_spawn(foo,RED);
    cout <<"[tester] spawn should not return -1: " << suspend << endl;
    int ter = uthread_spawn(foo,GREEN);
    cout <<"[tester] spawn should not return -1: " << ter << endl;
    int ready = uthread_spawn(goo,ORANGE);
    cout <<"[tester] spawn should not return -1: " <<ready << endl;

    cout <<"suspending" <<endl;
    uthread_suspend(suspend);
    cout <<"terminating" <<endl;
    uthread_terminate(ter);

    cout << "wrong suspend: " << uthread_suspend(ter) << endl;
    cout << "wrong terminate: " << uthread_terminate(ter)<< endl;
    cout << "wrong resume: " << uthread_resume(ter)<< endl;
    cout << "wrong get quantums: " << uthread_get_quantums(ter)<< endl;
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
//	uthread_init(QUANTUM_LENGTH);
//	test1();
//	test2();
//	test3();
//  test4(); //pass
//  test5(); // pass
//  test6(); //pass
//  test7(); //failed sometimes ?!?!
  test8(); // pass
//	test9();
////
//  err1();//pass
//  err2();//pass
// err3(); //pass
//  err4();//pass
//err5(); //fail


	while(true)
	{
		if (uthread_get_total_quantums() > totalQuantums)
		{
			cout<< "thread #: " << uthread_get_tid()  << ". main: "<<uthread_get_quantums(uthread_get_tid()) <<endl;
			totalQuantums = uthread_get_total_quantums();
		}
		if (uthread_get_quantums(uthread_get_tid()) > RUNS + 1)
		{
            uthread_terminate(uthread_get_tid());
		}

//		cout<<"main suspending goo"<<endl;
//		uthread_suspend(2);
	}

	return 0;
}



