//ScrambleClass.cpp

#include "stdafx.h"
#include "ScrambleClass.h"

using namespace std;

ScrambleClass::ScrambleClass(string *keystream):Solver(keystream)
{
	char temp;

	//READ LETTER COUNTS
	ifstream inFile ("ScrambleClass.ini");
	for(int i = 0; i < 26; i++)
	{
		inFile>>temp;
		inFile>>temp;

		inFile>>(int)letterCounts[i];
	}

	//INITIALIZE KEY STRING
	key = "";
	for(int i = 0; i < 26; i++)
		for(int l = 0; l < letterCounts[i]; l++)
			key += (char)(i + 65);
}

void ScrambleClass::run()
{
	int letterOne;
	int letterTwo;
	
	char temp;

	float tempScore;

	string tempSolution;

	//SEED RAND
	time_t seconds;
	time(&seconds);
	srand((unsigned int) seconds);

	//RUN SCRAMBLE
	for(int l = 0; l < 63; l++)
	{
		letterOne = (rand() % 63);
		letterTwo = (rand() % 63);

		temp = key[letterOne];
		key[letterOne] = key[letterTwo];
		key[letterTwo] = temp;
	}

	*KeyStream = key;
	cout << "KEY: " << key << endl;
	cout << "KEYSTREAM: " << *KeyStream << endl;
}