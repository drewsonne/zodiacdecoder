#include <iostream>
#include <string>
#include <cstdlib> 
#include <stdlib.h>
#include <ctime> 
#include <fstream>
#include <sstream>
#include "stdafx.h"
#include <time.h>
#include <math.h>

#ifndef SOLVER_H
#define SOLVER_H

using namespace std;

public class Solver
{
	public:
		Solver();
		virtual void run() = 0;
		long score(string key);
		bool sendKey(string key);
		stringstream KeyStream;
		string stdkey;
};

#endif
