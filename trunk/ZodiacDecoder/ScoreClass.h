// scoreClass.h
// header file for scoring algorithm class

// file requirements:
//   scoreClass.cpp
#pragma once
#include "stdafx.h"
#include "hash_table.h"
//#include "Client.h"
#define MAX_WORD_SCORE 101
using namespace std;



class ScoreClass
{
	public:
	
		ScoreClass();

		string SolveKey(string key);

		float ScoreSolution(string solution);

		//nGraphData
		int BI_GRAPHS[676];
		int TRI_GRAPHS[17576];
		int TETRA_GRAPHS[456976];
		int PENTA_GRAPHS[11881376];

	private:

		//cipher data
		string cipher;
		string uniques;

		void ReadCipher();

		//dictionary
		int Scores[MAX_WORD_SCORE];
		CHashTable<int> Dictionary;

		//letterData
		double letterFreqs[26];

		//functions
		void ReadNGraphs();
		void InitializeDictionary();
		void ReadLetterFreqs();
};

