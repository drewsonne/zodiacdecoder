#include "stdafx.h"
#include "Solver.h"

#ifndef IBF_H
#define IBF_H

using namespace std;

class IBF: public Solver{
	public:
		IBF(string *keystream);
		void run();
		void printibfKey();

	private:
		bool isFull();
		unsigned long sm;
		void printSolved();
		bool fit(long token, int position);
		unsigned long fill(unsigned long seedModifier);

};
#endif