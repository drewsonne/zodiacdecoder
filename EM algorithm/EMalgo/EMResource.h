#ifndef INCLUDED_EM
#include "main.cpp"
#define INCLUDED_EM
#endif

bool readModelerFile(const string&, SBModel&, string&);
bool readConfigFile(const string&, string&, string&, string&);
bool readCipherFile(const string&, SCipher&, string&);
bool displayError(const string&);
void displaySolutionTable(const SSolutionTable&);