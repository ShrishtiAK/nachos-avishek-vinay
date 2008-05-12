#ifndef PROFSTUDENTPROBLEM_H
#define PROFSTUDENTPROBLEM_H

#include "synch.h"

void ProfessorStudentCreate(int nSutdents);
Lock Speak("Lock needed to speak"); //Mutex to mutually excluse students
Lock ProfessorLock("Lock needed to speak"); //Mutex to mutually excluse students and professor
Semaphore Question("Lock to ask question", 0); //semaphore to ask question
Semaphore Answer("Lock to answer the question", 0); //semaphore to answer

int nQuestionNumber = 0; //Question number currently asked 0 - means no question pending
int nAnswerNumber = 0; //Answer number currently answered 0 means no answer pending

void QuestionStart(int nMyNumber);
void QuestionStart(int nMyNumber);
void AnswerStart(int nMyNumber);
void AnswerDone(int nMyNumber);
void Professor();
void Student();
#endif //PROFSTUDENTPROBLEM_H
