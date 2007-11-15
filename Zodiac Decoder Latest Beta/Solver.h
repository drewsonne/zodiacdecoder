#include "stdafx.h"

#ifndef SOLVER_H
#define SOLVER_H

using namespace std;

class Solver
{
	public:
		Solver(string *keystream);
		virtual void run() = 0;
		long score(string key);
		bool sendKey(string key);
		string *KeyStream;
};

#endif
