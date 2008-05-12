#include "profstudentproblem.h"
#include "system.h"

//Function to start asking a question
void QuestionStart(int nMyNumber)
{
	//Acquire the lock to ask the question
	Speak.Acquire();
	//Make sure that no other student asks question
	//while the professor is answering this question
	ProfessorLock.Acquire();
	//Flag the professor the question number to professor
	nQuestionNumber = nMyNumber;
	printf("Student: Question Start of %d \n", nMyNumber);
}

//Function to wait while professor answers the asked question
void QuestionDone(int nMyNumber)
{
	//Signal the sleeping professor about question
	Question.V();
	//Wait till professor answers question
	while(nMyNumber != nAnswerNumber)
	{
		Answer.P();
	}
	printf("Student: Question done, I got the answer for question %d\n\n", nMyNumber);
	//Question was answered release the lock for other students
	Speak.Release();
}


//Method to wait for any question
void AnswerStart()
{
	while(nQuestionNumber <= 0)
	{
		//wait while no questions are asked
		Question.P();
		
	}
	printf("Professor: Got a question %d \n", nQuestionNumber);
	
}

//Method to flag the student about answer being done
void AnswerDone()
{
	
	nAnswerNumber = nQuestionNumber;
	
	Answer.V();
	printf("Professor: Answer %d done\n", nAnswerNumber);
	
	nQuestionNumber = 0;
	
	ProfessorLock.Release();
	
}

//professor loops in this method
void Profesor(int)
{
	while(1)
	{
		
		currentThread->Yield();
		AnswerStart();
		
		printf("Professor: Answering the question %d \n", nQuestionNumber);
		
		AnswerDone();
		currentThread->Yield();
		
	}
}

//Student function
void Student(int n)
{
	while(1)
	{
		QuestionStart(n);
	
		printf("Student: Asking question %d\n", n);

		QuestionDone(n);
		currentThread->Yield();
	}

}

//Professor and student threads are created in this function
void ProfessorStudentCreate(int nSutdents)
{
	Thread *t1 = new Thread("forked male thread\n");
	
	t1->Fork(Profesor, 1);
	
	
	for(int i = 1 ; i <= nSutdents ;++i)
	{
		
		printf("starting student threads %d\n", i);
		
		Thread *t2 = new Thread("forked male thread");
		
		t2->Fork(Student, i);
		
	}
}
