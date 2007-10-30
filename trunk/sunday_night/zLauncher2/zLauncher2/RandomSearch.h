#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <math.h>

using namespace std;

ref class RandomSearch
{
public:
	RandomSearch();
	void run();
	System::String ^getKey();

private:
	System::String ^stdkey;
};
