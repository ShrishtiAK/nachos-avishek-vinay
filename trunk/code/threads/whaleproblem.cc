#include "whaleproblem.h"
#include "thread.h"
#include "system.h"


Lock Mate("Male lock"), FemaleLock("Female lock"), MatchMakerLock("Lock for match maker");
Semaphore MaleReady("Indicates male is ready", 0), FemaleReady("Indicates female is ready", 0);
Semaphore MaleDone("Mate done", 0), FemaleDone("Mate done", 0);
Lock Opportunity("Opportunity semaphore");
Condition CondOpportunity("opportunity to mate");

int CurMale = -1;
int CurFemale = -1;
int nMatingCount = 0;
bool bMatchMakerReady = false;
bool bMaleReady = false;
bool bFemaleReady = false;

void MatchMaker(int n, bool bIsMale = true)
{
	currentThread->Yield();
	MaleReady.P();
	currentThread->Yield();
	FemaleReady.P();
	currentThread->Yield();
	printf("Mating Counter: %d - Mating is taking place between the male whale %d and female whale %d and the match maker is: ", ++nMatingCount, CurMale, CurFemale);
	currentThread->Yield();
	if(bIsMale)
	{
		currentThread->Yield();
		printf("Male whale %d\n", n);
		currentThread->Yield();
	}
	else
	{
		currentThread->Yield();
		printf("Female whale %d\n", n);
		currentThread->Yield();
	}
	Mate.Acquire();	
	currentThread->Yield();
	bMaleReady = false;
	currentThread->Yield();
	bFemaleReady = false;
	currentThread->Yield();
	bMatchMakerReady = false;
	currentThread->Yield();
	Mate.Release();
	currentThread->Yield();
	MaleDone.V();
	currentThread->Yield();
	FemaleDone.V();
	currentThread->Yield();
	CondOpportunity.Broadcast(&Opportunity);
	currentThread->Yield();
}

void Male(int n)
{
	currentThread->Yield();
	while(1)
	{
		currentThread->Yield();
		while(bMatchMakerReady &&
				bMaleReady)
		{
			currentThread->Yield();
			CondOpportunity.Wait(&Opportunity);
			currentThread->Yield();
		}
		currentThread->Yield();
		Mate.Acquire();
		currentThread->Yield();
		if(!bMaleReady)
		{
			currentThread->Yield();
			bMaleReady = true;
			currentThread->Yield();
			Mate.Release();
			currentThread->Yield();
			CurMale = n;
			currentThread->Yield();
			MaleReady.V();
			currentThread->Yield();
			currentThread->Yield();
			MaleDone.P();
			currentThread->Yield();
		//	printf("Male after mating\n");
		//	Mate.Acquire();
		//	Mate.Release();
			break;
			currentThread->Yield();
		}
		else if(!bMatchMakerReady)
		{
			currentThread->Yield();
			bMatchMakerReady = true;
			currentThread->Yield();
			Mate.Release();
			currentThread->Yield();
			MatchMaker(n);
			currentThread->Yield();
		}
		else
		{
			currentThread->Yield();
			Mate.Release();
			currentThread->Yield();
		}
	}
}

void Female(int n)
{
	while(1)
	{
		while(bMatchMakerReady &&
				bFemaleReady)
		{
			currentThread->Yield();
			CondOpportunity.Wait(&Opportunity);
			currentThread->Yield();
		}
		currentThread->Yield();
		Mate.Acquire();
		currentThread->Yield();
		if(!bFemaleReady)
		{
			currentThread->Yield();
			bFemaleReady = true;
			currentThread->Yield();
			currentThread->Yield();
			Mate.Release();
			currentThread->Yield();
			currentThread->Yield();
			CurFemale = n;
			currentThread->Yield();
			FemaleReady.V();
			currentThread->Yield();
			FemaleDone.P();
			currentThread->Yield();
			break;
		}
		else if(!bMatchMakerReady)
		{
			currentThread->Yield();
			bMatchMakerReady = true;
			currentThread->Yield();
			Mate.Release();
			currentThread->Yield();
			currentThread->Yield();
			MatchMaker(n, false);
			currentThread->Yield();
		}
		else
		{
			currentThread->Yield();
			Mate.Release();
			currentThread->Yield();
		}
	}
}



void CWhale::testthread(int x)
{
	printf("%d", x);
}

void WhaleMatchMaker(int nMales, int nFemales)
{
	currentThread->Yield();
	for(int i = 1 ; i <= nMales ;++i)
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
	for(int j = 1 ; j <= nFemales ;++j)
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
