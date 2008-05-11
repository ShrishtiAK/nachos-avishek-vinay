#include "profstudentproblem.h"
#include "system.h"

void QuestionStart(int nMyNumber)
{
	Speak.Acquire();
	currentThread->Yield();
	ProfessorLock.Acquire();
	currentThread->Yield();
	nQuestionNumber = nMyNumber;
	printf("Question Start of %d \n", nMyNumber);
	currentThread->Yield();
}

void QuestionDone(int nMyNumber)
{
	currentThread->Yield();
	Question.V();
	//ProfessorLock.Release();
	currentThread->Yield();
	while(nMyNumber != nAnswerNumber)
	{
		currentThread->Yield();
		Answer.P();
		currentThread->Yield();
	}
	currentThread->Yield();
	printf("Question done, I got the answer for question %d\n", nMyNumber);
	Speak.Release();
	currentThread->Yield();
}

void AnswerStart()
{
	nAnswerNumber = 0;
	currentThread->Yield();
	while(nQuestionNumber <= 0)
	{
		currentThread->Yield();
		Question.P();
		currentThread->Yield();
	}
	printf("Got a question %d \n", nQuestionNumber);
	currentThread->Yield();
	//ProfessorLock.Acquire();
	currentThread->Yield();
}

void AnswerDone()
{
	currentThread->Yield();
	nAnswerNumber = nQuestionNumber;
	currentThread->Yield();
	Answer.V();
	printf("Answer %d done\n", nAnswerNumber);
	currentThread->Yield();
	nQuestionNumber = 0;
	currentThread->Yield();
	ProfessorLock.Release();
	currentThread->Yield();
}

void Profesor(int)
{
	while(1)
	{
		currentThread->Yield();
		AnswerStart();
		currentThread->Yield();
		printf("Answering the question %d \n", nQuestionNumber);
		currentThread->Yield();
		AnswerDone();
		currentThread->Yield();
	}
}

void Student(int n)
{
	//while(1)
	//{
	currentThread->Yield();
		QuestionStart(n);
	currentThread->Yield();	
		printf("Asking question %d\n", n);
	currentThread->Yield();
		QuestionDone(n);
	currentThread->Yield();
	//}
}

void ProfessorStudentCreate(int nSutdents)
{
	Thread *t1 = new Thread("forked male thread\n");
	currentThread->Yield();
	t1->Fork(Profesor, 1);
	
	
	for(int i = 1 ; i <= nSutdents ;++i)
	{
		currentThread->Yield();
		printf("starting student threads %d\n", i);
		currentThread->Yield();
		Thread *t2 = new Thread("forked male thread");
		currentThread->Yield();
		t2->Fork(Student, i);
		currentThread->Yield();
		//delete t;
	}
}
