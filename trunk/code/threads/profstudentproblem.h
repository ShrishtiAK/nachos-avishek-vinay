#ifndef PROFSTUDENTPROBLEM_H
#define PROFSTUDENTPROBLEM_H

#include "synch.h"

Lock Speak("Lock needed to speak");
Lock ProfessorLock("Lock needed to speak");
Semaphore Question("Lock to ask question", 0);
Semaphore Answer("Lock to answer the question", 0);

int nQuestionNumber = 0;
int nAnswerNumber = 0;

void QuestionStart(int nMyNumber);
void QuestionStart(int nMyNumber);
void AnswerStart(int nMyNumber);
void AnswerDone(int nMyNumber);
void ProfessorStudentCreate(int nSutdents);

#endif //PROFSTUDENTPROBLEM_H
