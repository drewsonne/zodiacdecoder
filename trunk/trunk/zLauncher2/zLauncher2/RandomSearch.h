#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <math.h>

using namespace std;

class RandomSearch
{
public:
	RandomSearch();
	void run();
	string getKey();

private:
	string stdkey;
};
