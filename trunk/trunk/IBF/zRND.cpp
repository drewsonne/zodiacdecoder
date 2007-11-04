// zRND.cpp - random solver
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <math.h>
using namespace std;

// keystream that reader will pull from.
// comment out for release
stringstream KeyStream (stringstream::in | stringstream::out);
void run();

int main()
{
	run();
	return 0;
}
void run()
{
	int SIMULATIONS=10000;
	// to this from stream
	string key;
	// iterate for n simulations
	for(int s = 1; s <= SIMULATIONS; s++)
	{
		// send random key to stream
		for(int j = 0; j < 63; j++){
			KeyStream<<(char)(65 + (rand() % 26));
		}
		// test reading from keystream
		KeyStream>>key;
		// comment out for release
		cout<<key<<" "<<(int)(((double)s / (double)SIMULATIONS) * 100)<<"%"<<endl;
		KeyStream.clear();
	}
}