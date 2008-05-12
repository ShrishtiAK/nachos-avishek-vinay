#include "producerconsumer.h"
#include "string.h"
#include "system.h"

const int CProducerConsumer::K_MAX_BUF_SIZE = 20;
const char CProducerConsumer::InputString[12] = "Hello World";
char CProducerConsumer::m_Buffer[K_MAX_BUF_SIZE] = "\0";
CProducerConsumer t;
CProducerConsumer::CProducerConsumer():
m_CanProduce("Can produce", K_MAX_BUF_SIZE),
m_CanConsume("Can consume", 0),
m_Producer_Mutex("producer mutex", 1),
m_Consumer_Mutex("consumer mutex", 1), m_ProducerIndex(0),
m_ConsumerIndex(0){}
	
static void ProducerWrapper(int n)
{
	printf("Producer starting\n");
	printf("Producer starting\n");
	t.Producer();
}

static void ConsumerWrapper(int n)
{
	//CProducerConsumer t;
	printf("consumer starting\n");
	t.Consumer();
}

void CProducerConsumer::Producer()
{
	printf("Producer starting\n");
	while(1)
	{
		for(unsigned int i = 0 ; i < strlen(InputString); ++i)
		{
			m_CanProduce.P();
			m_Producer_Mutex.P();
			char item = InputString[i];
			m_Buffer[m_ProducerIndex] = item;		
			m_ProducerIndex = (m_ProducerIndex + 1) % K_MAX_BUF_SIZE;
			m_Producer_Mutex.V();
			m_CanConsume.V();
		}
		
	}
}

void CProducerConsumer::Consumer()
{
	printf("condumer starting\n");
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

char CProducerConsumer::Produce()
{
	return 'F';
}

void CProducerConsumer::Consume(char c)
{
	printf("%c", c);
}

void CProducerConsumer::StartThreads(int nProduces, int nConsumers)
{
	m_pConsumerThreads = NULL;
	m_pProducerThreads = NULL;
	for(int i = 0 ; i < nProduces ;++i)
	{
		printf("starting producer threads %d\n", i);
		//Consumer();
		//Producer();
    	m_pProducerThreads[i] = new Thread("forked producer thread");
		m_pProducerThreads[i]->Fork(ProducerWrapper, i);
		//delete t;
	}
	for(int i = 0 ; i < nConsumers ;++i)
	{
		printf("starting consumer threads %d\n", i);
		m_pConsumerThreads[i] = new Thread("forked consumer thread");
		m_pConsumerThreads[i]->Fork(ConsumerWrapper, i);
		//delete t;
	}
	printf("started all the threads");
	//currentThread->Sleep();
}
