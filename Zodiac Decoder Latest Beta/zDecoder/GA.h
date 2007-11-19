#ifndef GA_H
#define GA_H

#include "stdafx.h"
#include "Solver.h"
#include "ScoreClass.h"

#define _ASCII_SIZE 256
#define _MAX_KEYS 10000
#define _MAX_BONUS 1000
#define _MAX_CIPHER 1000
#define _MAX_GENERATIONS 1000000

using namespace std;

class GA: public Solver
{
	public:
		GA(string *keystream, ScoreClass *scorer);
		void run();
		void start();

		int generation;
		int GENERATIONS;

	private:
		ScoreClass *myScorer;

		struct keyType
		{
			bool solved;

			float score;

			string key;
			string solution;
		};
	
		int CIPHER_SIZE;
		int CIPHER_WIDTH;
		int CIPHER_HEIGHT;
		int KEY_SIZE;
		int NUM_KEYS;
		

		float SURVIVAL_RATE;
		float BREED_RATE;
		float MUTATE_RATE;

		bool seedKeys;
		string seed;

		keyType keys[_MAX_KEYS];
		keyType bestKey;

		//SORT VARS
		keyType temp;
		int lowestLoc;
		float lowestScore;
		int countdown;

		//MATE AND MUTATE VARS
		int donor;
		int mother; 
		int father; 
		int mutant;
		int mutation;
		int numMutations;
		int mutationLocation;
	   
		int survivingPopulation;
		int numChildren;
		int numMutants;

		string mutantString;

		keyType newKey;
};

#endif