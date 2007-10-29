#ifndef GA_H
#define GA_H

#include <iostream>
#include <string>
#include <cstdlib> 
#include <stdlib.h>
#include <ctime> 
#include <fstream>
#include <time.h>
#include <math.h>
#include "include\hash_table.h"
#include "Solver.h"

#define _ASCII_SIZE 256
#define _MAX_KEYS 10000
#define _MAX_BONUS 1000
#define _MAX_CIPHER 1000
#define _MAX_GENERATIONS 1000000

#define _NUM_ALGOS 1000

#define DICTIONARY_FILE "words.txt"
#define MAX_WORD_SCORE 1000

using namespace std;

class GA: public Solver
{
	public:
		GA();
		void run();
		//int main();


	private:
		struct keyType
		{
			float score;

			char key[_ASCII_SIZE];
			char solution[_MAX_CIPHER];
		};

		struct scoreType
		{
			double biWeight;
			double triWeight;
			double tetraWeight;
			double pentaWeight;

			double nGraphWeight;
			double wordWeight;
			double letterWeight;

			double biGraphScore;
			double triGraphScore;
			double tetraGraphScore;
			double pentaGraphScore;

			double nGraphScore;
			double wordScore;
			double letterScore;

			double score;
		};


		int BI_GRAPHS[676];
		int TRI_GRAPHS[17576];
		int TETRA_GRAPHS[456976];
		int PENTA_GRAPHS[11881376];

		char CIPHER[_MAX_CIPHER];
		char UNIQUES[_ASCII_SIZE];

		bool rePrint;

		bool seedKeys;
		string seed;

		double letterFreqs[26];

		keyType keys[_MAX_KEYS];

		//dictionary
		int Scores[MAX_WORD_SCORE + 1];
		CHashTable<int> Dictionary;

		void Defaults(int &, int &, int &, int &, int &, int &, bool &, string &, double &, double &, double &, double &, double &, double &, double &);
		void Simulate(int, string, int, int, int, int, int, double, double, double, double, double, double, double);
		void Simulations(int, string, int, int, int, int, int, double, double, double, double, double, double, double);
		void InitializeKeys(int, int);
		void InitializeDictionary();
		void SolveKeys(int, int, int);
		void ScoreKeys(int, int, int, keyType &, double, double, double, double);
		void SortKeys(int);
		void MateAndMutate(int, int, double, double, double);
		void PrintCipher(keyType key, int, int, int);

		void ReadCipher(string, int, int);
		void ReadNGraphs(bool LOGWEIGHT);

		void Intro();
		void PrintMenu();
		void Configure(int &NUM_KEYS, string &CIPHER_FILE, int &CIPHER_SIZE, int &KEY_SIZE, int &GENERATIONS, bool &LOGWEIGHT, int &CIPHER_HEIGHT, int &CIPHER_WIDTH, double &SURVIVAL_RATE, double &BREED_RATE, double &MUTATE_RATE, double &BI_BONUS, double &TRI_BONUS, double &TETRA_BONUS, double &PENTA_BONUS);


};

#endif