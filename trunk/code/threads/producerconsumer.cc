#include "producerconsumer.h"
#include "string.h"
#include "system.h"

const int CProducerConsumer::K_MAX_BUF_SIZE = 20; //Size of the buffer shared
const char CProducerConsumer::InputString[12] = "Hello World"; 
char CProducerConsumer::m_Buffer[K_MAX_BUF_SIZE] = "\0";   //Shared buffer among the producers and consumer
CProducerConsumer ProducerConsumer;
CProducerConsumer::CProducerConsumer(): 
m_CanProduce("Can produce", K_MAX_BUF_SIZE),
m_CanConsume("Can consume", 0),
m_Producer_Mutex("producer mutex", 1),
m_Consumer_Mutex("consumer mutex", 1), m_ProducerIndex(0),
m_ConsumerIndex(0){}
	
static void ProducerWrapper(int n)
{
	printf("Producer starting\n");
	ProducerConsumer.Producer();
}

static void ConsumerWrapper(int n)
{
	printf("consumer starting\n");
	ProducerConsumer.Consumer();
}

void CProducerConsumer::Producer()
{
	while(1)
	{
		m_CanProduce.P();
		m_Producer_Mutex.P();
		m_Buffer[m_ProducerIndex] = Produce();		
		m_ProducerIndex = (m_ProducerIndex + 1) % K_MAX_BUF_SIZE;
		m_Producer_Mutex.V();
		m_CanConsume.V();
	}
}

void CProducerConsumer::Consumer()
{
	while(1)
	{
		m_CanConsume.P();
		m_Consumer_Mutex.P();
		char item = m_Buffer[m_ConsumerIndex];
		m_ConsumerIndex = (m_ConsumerIndex + 1) % K_MAX_BUF_SIZE;
		m_CanProduce.V();
		Consume(item);
		m_Consumer_Mutex.V();
	}
}

//returns 1 char at a time from the string "Hello World"
char CProducerConsumer::Produce()
{
	static int i = 0;
	char c = InputString[i];
	i = (i + 1) % strlen(InputString);
	return c;
}

void CProducerConsumer::Consume(char c)
{
	printf("%c", c);
}

//Starts the producer and consumer threads
void CProducerConsumer::StartThreads(int nProduces, int nConsumers)
{
	for(int i = 0 ; i < nProduces ;++i)
	{
		printf("starting producer threads %d\n", i);
		//Consumer();
		//Producer()
    	Thread* t1 = new Thread("forked producer thread");
		t1->Fork(ProducerWrapper, i);
		//delete t;
	}
	for(int i = 0 ; i < nConsumers ;++i)
	{
		printf("starting consumer threads %d\n", i);
		Thread* t2 = new Thread("forked consumer thread");
		t2->Fork(ConsumerWrapper, i);
		//delete t;
	}
	printf("started all the threads");
	//currentThread->Sleep();
}
