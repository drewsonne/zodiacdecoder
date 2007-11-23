 // GA.cpp - MAIN FILE FOR OUR ZODIAC 340 GENETIC ALGORITHM CLIENT

/////
// INCLUDES
/////

#include "stdafx.h"
#include "GA.h"
using namespace std;

GA::GA(string *keystream, ScoreClass *scorer):Solver(keystream)
{
	/////
	// SIMULATION PARAMETERS
	/////
	CIPHER_SIZE = 340;
	CIPHER_WIDTH = 17;
	CIPHER_HEIGHT = 20;
	KEY_SIZE = 63;
	NUM_KEYS = 100;
	GENERATIONS = 100;

	SURVIVAL_RATE = 0.1;
	BREED_RATE = 0.6;
	MUTATE_RATE = 1.0 - BREED_RATE;
	/////

	//MATE AND MUTATE VARS
	survivingPopulation = (int)((double)NUM_KEYS * SURVIVAL_RATE);
	numChildren = (int)((double)(NUM_KEYS - survivingPopulation) * BREED_RATE);
	numMutants = (int)((double)(NUM_KEYS - survivingPopulation) * MUTATE_RATE);

	time_t seconds;
	time(&seconds);

	srand((unsigned int) seconds);

	myScorer = scorer;
	start();
}

void GA::start()
{
	generation = 0;

	//INITIALIZE BEST KEY
	bestKey.key = "";
	bestKey.solution = "";
	bestKey.score = 0;
	bestKey.solved = true;

	//INITIALIZE KEYS
	for(int k = 0; k < NUM_KEYS; k++)
	{
		keys[k].solved = false;
		keys[k].score = 0;
		keys[k].key = "";

		for(int c = 0; c < KEY_SIZE; c++)
			keys[k].key += (char)(65 + (rand() % 26));
	}
}

void GA::run()
{
	//RUN SIMULATION

	cout<<"GEN: "<<generation<<endl;

	//SOLVE KEYS
	for(int k = 0; k < NUM_KEYS; k++)
		{
			if(!keys[k].solved)
				keys[k].solution = myScorer->SolveKey(keys[k].key);
		}
	
		//SCORE KEYS
		for(int k = 0; k < NUM_KEYS; k++)
		{
			if(keys[k].score == 0)
				keys[k].score = myScorer->ScoreSolution(keys[k].solution);


			if(keys[k].score > bestKey.score)
			{
				bestKey = keys[k];
				
				//RESET SIM
				generation = 0;
			}

			cout << "keyk.sscore " << keys[k].score << endl;
		}
	
		//SORT KEYS
		countdown = NUM_KEYS - 1;

		for(int a = 0; a < NUM_KEYS; a++)
		{
			lowestScore = 101;

			for(int u = 0; u < countdown; u++)
				if(keys[u].score < lowestScore)
				{
					lowestLoc = u;
					lowestScore = keys[u].score;
				}

			temp = keys[countdown];
			keys[countdown] = keys[lowestLoc];
			keys[lowestLoc] = temp;

			countdown--;
		}

		//BREED KEYS
		for(int i = survivingPopulation; i < survivingPopulation + numChildren - 1; i++)
		{                
			newKey.key = "";

			mother = (rand() % survivingPopulation);
			father = (rand() % survivingPopulation);

			for (int c = 0; c < KEY_SIZE; c++) 
			{
				donor = rand() % 2;
                
				if (donor == 0) 
					newKey.key += keys[mother].key[c];
				else 
					newKey.key += keys[father].key[c];
			}

			newKey.score = 0;
			newKey.solved = false;

			keys[i] = newKey;
		}

		//MUTATE KEYS
		for(int i = NUM_KEYS - numMutants - 1; i < NUM_KEYS; i++)
		{
			mutant = rand() % survivingPopulation;
			mutantString = keys[mutant].key;

			numMutations = (rand() % KEY_SIZE) + 1;

			for(int m = 0; m < numMutations; m++)
			{
				mutation = (rand() % 26) + 65;

				mutationLocation = rand() % KEY_SIZE;

				mutantString[mutationLocation] = mutation;
			}

			keys[i].key = mutantString;

			keys[i].score = 0;
		}

		*KeyStream = bestKey.key;
		cout << "KEY: " << bestKey.key << endl;
		cout << "KEYSTREAM: " << *KeyStream << endl;

		generation++;
		if(generation > GENERATIONS)
			start();
}