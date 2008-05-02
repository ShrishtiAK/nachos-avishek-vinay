// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "synch.h"

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest()
{
    DEBUG('t', "Entering SimpleTest");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}
Lock SemaphoreLock("Lock test");
int x=0;
void LockUnlock(const int i)
{
	SemaphoreLock.Acquire();
	x++;
	printf("value of x = %d\n", x);
	if(SemaphoreLock.isHeldByCurrentThread())
	{
		printf("Lock is held by current thread - Thread %d\n", i);
		for(int j = 0; j < 65535 ; ++j){}
	}
	SemaphoreLock.Release();
}
void SynchTest()
{

	for(int i = 0 ; i < 20 ;++i)
	{
    	Thread *t = new Thread("forked thread");
		t->Fork(LockUnlock, i);
		//delete t;
	}

}
