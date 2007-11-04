#include "stdafx.h"
#include <iostream>
#include <string>
#include <cstdlib> 
#include <stdlib.h>
#include <ctime> 
#include <fstream>
#include "Solver.h"


#ifndef IBF_H
#define IBF_H

using namespace std;

class IBF: public Solver{
	public:
		IBF(void);
		void run();
		System::String ^getKey();
		void printibfKey();
		stringstream KeyStream;

	private:
		static const unsigned long t = 4897367; //size of ibfToken library
		static const unsigned short c = 340; //size of cypher
		static const unsigned short k = 63; //size of ibfKey
		static const unsigned short n = 3; //size of ibfTokens/n-grams
		int ibfCipher[c];
		string ibfToken[t];
		string ibfKey[k];

		bool isFull();
		void printSolved();
		bool fit(long token, int position);
		void fill();

};
#endif