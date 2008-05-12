#ifndef WHALEPROBLEM_H
#define WHALEPROBLEM_H

#include "synch.h"
static Lock Mate("Male lock"); //Lock to control whoc can 
static Semaphore MaleReady("Indicates male is ready", 0), FemaleReady("Indicates female is ready", 0);
static Semaphore MaleDone("Mate done", 0), FemaleDone("Mate done", 0);
static Lock Opportunity("Opportunity semaphore");
static Condition CondOpportunity("opportunity to mate");

//Identifiers for male and female whales
static int CurMale = -1;
static int CurFemale = -1;
//Count to maintain the number of mates so far
static int nMatingCount = 0;
//Flags controlling the match maker
static bool bMatchMakerReady = false;
static bool bMaleReady = false;
static bool bFemaleReady = false;

void WhaleMatchMaker(int nMales, int nFemales);
void ResetData();

#endif //WHALEPROBLEM_H
