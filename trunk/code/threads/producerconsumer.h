#ifndef PRODUCER_CONSUMER_H
#define PRODUCER_CONSUMER_H

#include "synch.h"
//class Semaphore;

	
class CProducerConsumer
{
	public:
		CProducerConsumer();
		~CProducerConsumer(){}
		void StartThreads(int nProduces, int nConsumers);
		void Producer();
		void Consumer();
	private:
		
		char Produce();
		void Consume(char c);
		static const int K_MAX_BUF_SIZE;
		static const char InputString[];
		Semaphore m_CanProduce;
		Semaphore m_CanConsume;
		Semaphore m_Producer_Mutex;
	    Semaphore m_Consumer_Mutex;
		static char m_Buffer[];
		int m_ProducerIndex;
		int m_ConsumerIndex;
		Thread **m_pProducerThreads;
		Thread **m_pConsumerThreads;
};
#endif //PRODUCER_CONSUMER_H
