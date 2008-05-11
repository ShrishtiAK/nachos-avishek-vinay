/*Implementation of the problem 13 of the assignment #1*/


#include "boatproblem.h"
#include "thread.h"
#include "system.h"


/*SelectBoatSeat
 * Description: Function to select a seat in the boat
 * @param nNumber int identifier for the thread.
 * @param Ctype char character which tells the type of thread
 * @return void
 */

void SelectBoatSeat(int nNumber, char cType)
{
	int nIndex = (nMissionaryCount + nCannibalCount) - 1;
	printf("%c %d selecting seat %d\n",cType, nNumber, nIndex);
	nArrayBoat[nIndex] = nNumber;
	cArrayType[nIndex] = cType;
}

/*ResetValues
 *Description: resets all the data values used
 *@return void 
 */
void ResetValues()
{
	bWaitForMissionaryChance = bWaitForCannibalChance= false;	
	currentThread->Yield();
	nCannibalCount = nMissionaryCount = 0;
}

/* RowBoat
 * Description: Function to row a boat, called by one of the missionary or 
 * cannibal thread when the boat can safely row
 * @return void
 */
void RowBoat()
{
	printf("Row number: %d Rowing Boat with %c%d + %c%d + %c%d \n",
	++NumberOfRows, cArrayType[0],  nArrayBoat[0],cArrayType[1],
	nArrayBoat[1],cArrayType[2],  nArrayBoat[2]);
	ResetValues();
	currentThread->Yield();
	CondNextChance.Broadcast(&NextChance);
	currentThread->Yield();
	BoatMutex.Release();
	currentThread->Yield();
}


/*MissionaryArrives
 * Description: Function for all the missionary threads, it controls the 
 * way missionaries can get into boat
 * @param id integer value to identify the thread
 * @return void
 */
void MissionaryArrives(int id)
{
	while(1)
	{
		//Wait till the boat is free for a missionay to enter
		while(bWaitForMissionaryChance)
		{
			CondNextChance.Wait(&NextChance); //if boat is not free for now, wait till it is free
			currentThread->Yield();
		}
		//Boat seems to be available lets try and check if it can be taken
		BoatMutex.Acquire();
		currentThread->Yield();
		//If the entering of the current missionary satisfies the boat's capacity safely then just row the boat
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
