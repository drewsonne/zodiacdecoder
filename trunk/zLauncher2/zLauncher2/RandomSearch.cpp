#include "stdafx.h"
#include "RandomSearch.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <math.h>

//using namespace std;

stringstream KeyStream (stringstream::in | stringstream::out);
string stdkey;

RandomSearch::RandomSearch()
{
}

void RandomSearch::run()
{
	int SIMULATIONS=100;

	// iterate for n simulations
	for(int s = 1; s <= SIMULATIONS; s++)
	{
		// send random key to stream
		for(int j = 0; j < 63; j++){
			KeyStream<<(char)(65 + (rand() % 26));
		}
		KeyStream >> stdkey;
	}
	KeyStream.clear();
}

string RandomSearch::getKey()
{
	return stdkey;
}