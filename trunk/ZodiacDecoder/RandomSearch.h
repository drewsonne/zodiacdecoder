#pragma once
#include "stdafx.h"
#include "Solver.h"

using namespace std;

class RandomSearch: public Solver
{
public:
	RandomSearch(string *keystream);
	void run();

private:
	string stdkey;
};
