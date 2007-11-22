#include "stdafx.h"
#include "RandomSearch.h"

//using namespace std;

//stringstream KeyStream (stringstream::in | stringstream::out);
//string stdkey;

RandomSearch::RandomSearch(string *keystream):Solver(keystream)
{
}

string rndmtemp="";

void RandomSearch::run()
{
		rndmtemp.clear();
		
		for(int j = 0; j < 63; j++){
			rndmtemp+=(char)(65 + (rand() % 26));
		}

		// random key to keystream
		*KeyStream = rndmtemp;
}