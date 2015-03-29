#ifndef SCHEDULER_H_INCLUDED
#define SCHEDULER_H_INCLUDED



namespace scheduler
{
Thread* gRunning;
Thread* gBlocked[MAX_THREAD_NUM];
priorityList ready;

   static void init(*Thread);
   static void add(*Thread);

}


#endif // SCHEDULER_H_INCLUDED
