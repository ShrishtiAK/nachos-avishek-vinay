#include "whaleproblem.h"
#include "thread.h"
#include "system.h"

Lock MaleLock("Male lock"), FemaleLock("Female lock"), MatchMakerLock("Lock for match maker");
Semaphore MaleReady("Indicates male is ready", 0), FemaleReady("Indicates female is ready", 0);
Semaphore MaleDone("Mate done", 0), FemaleDone("Mate done", 0);
int CurMale = -1;
int CurFemale = -1;
void Male(int n)
{
	currentThread->Yield();
	MaleLock.Acquire();
	CurMale = n;
	currentThread->Yield();
	MaleReady.V();
	currentThread->Yield();
	currentThread->Yield();
	MaleDone.P();
}

void Female(int n)
{
	currentThread->Yield();
	FemaleLock.Acquire();
	currentThread->Yield();
	CurFemale = n;
	currentThread->Yield();
	FemaleReady.V();
	currentThread->Yield();
	FemaleDone.P();
	currentThread->Yield();
}

void MatchMaker(int n)
{
	while(1)
	{
		MatchMakerLock.Acquire();
		MaleReady.P();
		FemaleReady.P();
		printf("Mating is taking place between the male whale %d and female whale %d\n", CurMale, CurFemale);
		MaleDone.V();
		FemaleDone.V();
		MaleLock.Release();
		FemaleLock.Release();
		MatchMakerLock.Release();
		currentThread->Yield();
	}
}

void CWhale::testthread(int x)
{
	printf("%d", x);
}

void WhaleMatchMaker(int nMales, int nFemales)
{
	
			printf("starting matchmaker thread %d\n", 0);
		Thread *t1 = new Thread("forked mathch maker thread");
		t1->Fork(MatchMaker, 0);
		//delete t;
	currentThread->Yield();
	for(int i = 0 ; i < nMales ;++i)
	{
		currentThread->Yield();
		printf("starting male threads %d\n", i);
		currentThread->Yield();
		Thread *t2 = new Thread("forked male thread");
		currentThread->Yield();
		t2->Fork(Male, i);
		//delete t;
	}
	currentThread->Yield();
	for(int j = 0 ; j < nFemales ;++j)
	{
		currentThread->Yield();
		printf("starting female %d\n", j);
		currentThread->Yield();
		Thread *t2 = new Thread("forked female thread");
		currentThread->Yield();
		t2->Fork(Female, j);
		//delete t;
	}

}
