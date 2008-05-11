#include "boatproblem.h"
#include "thread.h"
#include "system.h"


void SelectBoatSeat(int nNumber, char cType)
{
	int nIndex = (nMissionaryCount + nCannibalCount) - 1;
	printf("%c %d selecting seat %d\n",cType, nNumber, nIndex);
	nArrayBoat[nIndex] = nNumber;
	cArrayType[nIndex] = cType;
}

void ResetValues()
{
	bWaitForMissionaryChance = bWaitForCannibalChance= false;	
	currentThread->Yield();
	nCannibalCount = nMissionaryCount = 0;
}

void RowBoat()
{
	printf("Row number: %d Rowing Boat with %c%d + %c%d + %c%d \n", ++NumberOfRows, cArrayType[0],  nArrayBoat[0],cArrayType[1],
	nArrayBoat[1],cArrayType[2],  nArrayBoat[2]);
	ResetValues();
	currentThread->Yield();
	CondNextChance.Broadcast(&NextChance);
	currentThread->Yield();
	BoatMutex.Release();
	currentThread->Yield();
}

void MissionaryArrives(int id)
{
	while(1)
	{
		while(bWaitForMissionaryChance)
		{
			CondNextChance.Wait(&NextChance);
			currentThread->Yield();
		}
		BoatMutex.Acquire();
		currentThread->Yield();
		if(nMissionaryCount && nCannibalCount ||
			nMissionaryCount == 2)
		{
			nMissionaryCount++;
			--nMissionariesLeft;
			SelectBoatSeat(id, 'M');
			RowBoat();
			break;
		}
		else if(nMissionariesLeft == 1 &&
				nMissionaryCount == 0)
		{
			bWaitForMissionaryChance = true;
			BoatMutex.Release();
			//currentThread->Yield();
			continue;
		}
		else if(nMissionaryCount)
		{
			++nMissionaryCount;
			--nMissionariesLeft;
			SelectBoatSeat(id, 'M');
			BoatMutex.Release();
			currentThread->Yield();
			CondBoatReady.Wait(&BoatReady);
			break;
		}
		else if(nCannibalCount)
		{
			++nMissionaryCount;
			--nMissionariesLeft;
			bWaitForCannibalChance = true;
			SelectBoatSeat(id, 'M');
			BoatMutex.Release();			
			currentThread->Yield();
			CondBoatReady.Wait(&BoatReady);
			break;
		}
		else
		{
			++nMissionaryCount;
			--nMissionariesLeft;
			SelectBoatSeat(id, 'M');
			BoatMutex.Release();
			currentThread->Yield();
			CondBoatReady.Wait(&BoatReady);
			break;
		}
		
	}
}

void CannibalArrives(int id)
{
	while(1)
	{
		while(bWaitForCannibalChance)
		{
			CondNextChance.Wait(&NextChance);			
			currentThread->Yield();
		}
		BoatMutex.Acquire();
		currentThread->Yield();
		if(nCannibalCount == 2||
		   nMissionaryCount == 2)
		{
			nCannibalCount++;
			--nCannibalsLeft;
			SelectBoatSeat(id, 'C');
			RowBoat();
			break;
		}
		else if(nCannibalCount == 1)
		{
			if(nCannibalsLeft == 1 || nMissionaryCount == 1)
			{
				bWaitForCannibalChance = true;
				BoatMutex.Release();
				//currentThread->Yield();
				continue;
			}
			else
			{
				++nCannibalCount;
				--nCannibalsLeft;
				bWaitForMissionaryChance = true;
				SelectBoatSeat(id, 'C');
				BoatMutex.Release();
				currentThread->Yield();
				CondBoatReady.Wait(&BoatReady);
				break;
			}
		}
		else if(nMissionaryCount)
		{
			++nMissionaryCount;
			--nCannibalsLeft;
			bWaitForCannibalChance = true;
			SelectBoatSeat(id, 'C');
			BoatMutex.Release();
			currentThread->Yield();
			CondBoatReady.Wait(&BoatReady);
			break;
		}
		else
		{
			++nCannibalCount;
			--nCannibalsLeft;
			SelectBoatSeat(id, 'C');
			BoatMutex.Release();
			currentThread->Yield();
			CondBoatReady.Wait(&BoatReady);
			break;
		}
		
	}
}

void CannibalMissionaryCreator(int nCannibals, int nMissionaries)
{
	nMissionariesLeft = nMissionaries;
	nCannibalsLeft = nCannibals;
	currentThread->Yield();
	for(int j = 1 ; j <= nMissionaries ;++j)
	{
		currentThread->Yield();
		printf("starting missionary %d\n", j);
		currentThread->Yield();
		Thread *t2 = new Thread("forked missionary thread");
		currentThread->Yield();
		t2->Fork(MissionaryArrives, j);
		//delete t;
	}

	currentThread->Yield();
	for(int i = 1 ; i <= nCannibals ;++i)
	{
		currentThread->Yield();
		printf("starting cannibal threads %d\n", i);
		currentThread->Yield();
		Thread *t2 = new Thread("forked cannibal thread");
		currentThread->Yield();
		t2->Fork(CannibalArrives, i);
		//delete t;
	}
	
	
	
	
	
	
}
