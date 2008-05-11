#ifndef BOATPROBLEM_H
#define BOATPROBLEM_H


#include "synch.h"

int nMissionaryCount = 0;
int nCannibalCount = 0;

int nMissionariesLeft = 0;
int nCannibalsLeft = 0;

bool bWaitForMissionaryChance = false;
bool bWaitForCannibalChance = false;

int nArrayBoat[3]={0,0,0};
char cArrayType[3]={'D', 'D', 'D'};

int NumberOfRows = 0;

void CannibalMissionaryCreator(int nCannibals, int nMissionaries);

Lock BoatMutex("Mutex for boat");
Lock NextChance("Mutex for Next chance");
Lock BoatReady("Lock to wait till boat is ready");

Condition CondBoatReady("Condition variable to wait till boat is ready");
Condition CondNextChance("Cond variable for next chance on boat");

void MissionaryArrives(int id);
void CannibalArrives(int id);
void RowBoat();
void ResetValues();
#endif //BOATPROBLEM_H
