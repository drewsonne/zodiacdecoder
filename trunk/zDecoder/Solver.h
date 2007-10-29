#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <string>
#include <cstdlib> 
#include <stdlib.h>
#include <ctime> 
#include <fstream>
#include <math.h>

using namespace std;

class Solver
{
	public:
		Solver();
		virtual void run() = 0;

	private:
		long score(string key);
		bool sendKey(string key);
};

#endif
