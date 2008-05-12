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
	
	BoatMutex.Release();
	
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
			
		}
		//Boat seems to be available lets try and check if it can be taken
		BoatMutex.Acquire();
		
		//If the entering of the current missionary satisfies the boat's capacity safely then just row the boat
		if(nMissionaryCount && nCannibalCount ||
			nMissionaryCount == 2)
		{
			nMissionaryCount++;
			--nMissionariesLeft;
			SelectBoatSeat(id, 'M');
			RowBoat();
			break;
		}//The last missionary cannot enter if it is first one to enter
		else if(nMissionariesLeft == 1 &&
				nMissionaryCount == 0)
		{
			bWaitForMissionaryChance = true;
			BoatMutex.Release();
			continue;
		}//If there is a missionary already enter the boat
		else if(nMissionaryCount)
		{
			++nMissionaryCount;
			--nMissionariesLeft;
			SelectBoatSeat(id, 'M');
			BoatMutex.Release();
			
			CondBoatReady.Wait(&BoatReady);
			break;
		}//If there is a Cannibal already in the boat and it is still not full then also enter
		else if(nCannibalCount)
		{
			++nMissionaryCount;
			--nMissionariesLeft;
			bWaitForCannibalChance = true;
			SelectBoatSeat(id, 'M');
			BoatMutex.Release();				
			CondBoatReady.Wait(&BoatReady);
			break;
		}
		else //When there is nobody in boat also enter the boat
		{
			++nMissionaryCount;
			--nMissionariesLeft;
			SelectBoatSeat(id, 'M');
			BoatMutex.Release();			
			CondBoatReady.Wait(&BoatReady);
			break;
		}
		
	}
}

//Cannibal function
void CannibalArrives(int id)
{
	while(1)
	{
		//Wait till the boat is free for a missionay to enter
		while(bWaitForCannibalChance)
		{
			CondNextChance.Wait(&NextChance); //if boat is not free for now, wait till it is free
			
		}
		//Boat seems to be available lets try and check if it can be taken
		BoatMutex.Acquire();
		
		//If the entering of the current cannibal satisfies the boat's capacity safely then just row the boat
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
			//The last missionary cannot enter if there is already a cannibal
			//And if there is a missionary already then also it cannot enter
			if(nCannibalsLeft == 1 || nMissionaryCount == 1)
			{
				bWaitForCannibalChance = true;
				BoatMutex.Release();
				continue;
			}//if it is 3rd cannibal also it can enter
			else
			{
				++nCannibalCount;
				--nCannibalsLeft;
				bWaitForMissionaryChance = true;
				SelectBoatSeat(id, 'C');
				BoatMutex.Release();
				
				CondBoatReady.Wait(&BoatReady);
				break;
			}
		}
		else if(nMissionaryCount)//If there a missionary already then safely enter
		{
			++nMissionaryCount;
			--nCannibalsLeft;
			bWaitForCannibalChance = true;
			SelectBoatSeat(id, 'C');
			BoatMutex.Release();
			
			CondBoatReady.Wait(&BoatReady);
			break;
		}
		else//if there no body also enter safely
		{
			++nCannibalCount;
			--nCannibalsLeft;
			SelectBoatSeat(id, 'C');
			BoatMutex.Release();
			
			CondBoatReady.Wait(&BoatReady);
			break;
		}
		
	}
}

//Method to create the missionay and cannibal threads
void CannibalMissionaryCreator(int nCannibals, int nMissionaries)
{
	nMissionariesLeft = nMissionaries;
	nCannibalsLeft = nCannibals;
	
	for(int j = 1 ; j <= nMissionaries ;++j)
	{
		
		printf("starting missionary %d\n", j);
		
		Thread *t2 = new Thread("forked missionary thread");
		
		t2->Fork(MissionaryArrives, j);
		//delete t;
	}

	
	for(int i = 1 ; i <= nCannibals ;++i)
	{
		
		printf("starting cannibal threads %d\n", i);
		
		Thread *t2 = new Thread("forked cannibal thread");
		
		t2->Fork(CannibalArrives, i);
		//delete t;
	}
	
	
	
	
	}
