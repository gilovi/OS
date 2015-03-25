#ifndef SCHEDULER_H_INCLUDED
#define SCHEDULER_H_INCLUDED



namespace scheduler
{
Thread gThreads[MAX_THREAD_NUM];
Thread* gRunning;
Thread* gBlocked[MAX_THREAD_NUM];
std::queue<Thread*> gRed;
std::queue<Thread*> gOrange;
std::queue<Thread*> gGreen;

   static void myMethod() ;

}


#endif // SCHEDULER_H_INCLUDED
