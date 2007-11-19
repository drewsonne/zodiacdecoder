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
	// to this from stream
	// iterate for n simulations
		// send random key to stream
		rndmtemp.clear();
		for(int j = 0; j < 63; j++){
			rndmtemp+=(char)(65 + (rand() % 26));
		}
		*KeyStream = rndmtemp;
		// test reading from keystream
		// comment out for release
		//cout<<key<<" "<<(int)(((double)s / (double)SIMULATIONS) * 100)<<"%"<<endl;

}

