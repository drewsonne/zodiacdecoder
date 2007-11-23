//ScrambleClass.h

#ifndef SCRAMBLE_CLASS_H
#define SCRAMBLE_CLASS_H

#include "stdafx.h"
#include "Solver.h"

using namespace std;

class ScrambleClass: public Solver
{
	public:

		ScrambleClass(string *keystream);

		void run();

	private:

		int letterCounts[26];

		string key;
};

#endif