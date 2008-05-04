#ifndef WHALEPROBLEM_H
#define WHALEPROBLEM_H

#include "synch.h"
class CWhale
{
	public:
		CWhale(){};
		~CWhale(){};
	void testthread(int x);
	private:
};

void WhaleMatchMaker(int nMales, int nFemales);

#endif //WHALEPROBLEM_H
