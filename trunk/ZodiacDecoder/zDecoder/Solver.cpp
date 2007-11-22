#include "stdafx.h"
#include "Solver.h"

using namespace std;

#define DICTIONARY_FILE "words.txt"

Solver::Solver(string *keystream)
{
	KeyStream = keystream;
}

bool Solver::sendKey(string key)
{
	return false;
}