/*Solution for problem 10*/

#include "whaleproblem.h"
#include "thread.h"
#include "system.h"


//MatchMaker: MatchMaker thread fucntion, waits till male and female whales are ready
//and then when they are ready, it prints that mating is taking place and releases the
//male and female
//Parameters are int n: identifier of the matchmaker threads, and bIsMale to determine
//the sex of the whale
void MatchMaker(int n, bool bIsMale)
{
	currentThread->Yield();
	//wait for male and female pair to be ready
	MaleReady.P();
	
	FemaleReady.P();
	
	//Mathing is taking place here
	printf("Mating Counter:%d -Mating is taking place between the male whale %d\
	and female whale %d and the match maker is: ",
	++nMatingCount, CurMale, CurFemale);
	
	if(bIsMale)
	{
		
		printf("Male whale %d\n", n);
		
	}
	else
	{
		
		printf("Female whale %d\n", n);
		
	}
	Mate.Acquire();	
	ResetData();
	Mate.Release();
	
	MaleDone.V();
	
	FemaleDone.V();
	
	CondOpportunity.Broadcast(&Opportunity);
	
}

void Male(int n)
{
	
	while(1)
	{
		//Wait till it is possible to become one of match maker or groom
		while(bMatchMakerReady &&
				bMaleReady)
		{
			
			CondOpportunity.Wait(&Opportunity);
			
		}
		//lock for mutual exclusion
		Mate.Acquire();
		//Every male whale first tries to become groom
		if(!bMaleReady)
		{
			
			bMaleReady = true;
			
			Mate.Release();
			
			CurMale = n;
			
			MaleReady.V();
			
			MaleDone.P();

			break;
			
		}//if there is already a groom then become match maker if there is vacancy
		else if(!bMatchMakerReady)
		{
			
			bMatchMakerReady = true;
			
			Mate.Release();
			
			MatchMaker(n, true);//female match maker
			
		}//After match making go back and wait for the next turn
		else //if neither can be done go back and wait
		{
			
			Mate.Release();
			
		}
	}
}

void Female(int n)
{
	while(1)
	{
		//Wait till it is possible to become one of match maker or groom
		while(bMatchMakerReady &&
				bFemaleReady)
		{
			
			CondOpportunity.Wait(&Opportunity);
			
		}
		
		//lock for mutual exclusion
		Mate.Acquire();
		//Every female whale first tries to become bride
		if(!bFemaleReady)
		{			
			bFemaleReady = true;			
			
			Mate.Release();			
			
			CurFemale = n;
			
			FemaleReady.V();
			
			FemaleDone.P();
			
			break;
		}//if there is already a bride then become match maker if there is vacancy
		else if(!bMatchMakerReady)
		{
			
			bMatchMakerReady = true;
			
			Mate.Release();			
			
			MatchMaker(n, false);
			
		}//After match making go back and wait for the next turn
		else //if neither can be done go back and wait
		{
			
			Mate.Release();
			
		}
	}
}


//Function to start the male and female threads
void WhaleMatchMaker(int nMales, int nFemales)
{
	
	for(int i = 1 ; i <= nMales ;++i)
	{
		
		printf("starting male threads %d\n", i);
		
		Thread *t2 = new Thread("forked male thread");
		
		t2->Fork(Male, i);
	}
	
	for(int j = 1 ; j <= nFemales ;++j)
	{
		
		printf("starting female %d\n", j);
		
		Thread *t2 = new Thread("forked female thread");
		
		t2->Fork(Female, j);
	}

}

//Method to reset the flags
void ResetData()
{
	bMaleReady = false;
	
	bFemaleReady = false;
	
	bMatchMakerReady = false;
}
