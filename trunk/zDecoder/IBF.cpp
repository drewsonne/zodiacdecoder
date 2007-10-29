#include <iostream>
#include <string>
#include <cstdlib> 
#include <stdlib.h>
#include <ctime> 
#include <fstream>
#include "IBF.h"

using namespace std;

const unsigned long t = 4897367; //size of ibfToken library
const unsigned short c = 340; //size of cypher
const unsigned short k = 63; //size of ibfKey
const unsigned short n = 3; //size of ibfTokens/n-grams

int ibfCipher[c];
string ibfToken[t];
string ibfKey[k];

IBF::IBF() {}


bool IBF::isFull(){
	for (int i = 0; i < 63; i++)
		if (ibfKey[i] == "*")
			return false;
	return true;
}

void IBF::printibfKey() {
	for (int i = 0; i < 63; i++)
		cout << ibfKey[i];
	cout << endl;
}

void IBF::printSolved() {
	for (int i = 0; i < 340; i++)
		cout << ibfKey[ibfCipher[i]];
	cout << endl;
}

bool IBF::fit(long token, int position) 
{
	int p;
	string temp[63];

	for(int z = 0; z < 63; z++)
		temp[z]=ibfKey[z];

	for(p = 0; p < n; p++)
	{
		if ( (ibfKey[ibfCipher[position + p]] != "*") && ((ibfKey[ibfCipher[position+p]]).at(0) != (ibfToken[token]).at(p)) )
		{
			return false;
		}
		temp[ibfCipher[position+p]] = (ibfToken[token]).at(p);
	}

	for(int z = 0; z < 63; z++)
		ibfKey[z]=temp[z];
	return true;

}


void IBF::fill(){
    string str;
    int i;
    bool placed;
    unsigned long randomT;
	const int x = c - (n - 1); //x = c - (n - 1)
	srand((unsigned)time(0)); 

	while(! isFull() )
	{
 		randomT = (rand() * rand() * rand()) % t;

		placed = false;
		i = 0;
		while ( (!placed) && (i < x) )
		{
			placed = fit(randomT, i);
			i++;
		}
	}
}


void IBF::run(){
	string str;
	int i =0;
	int x;
	ifstream ibfTokens("trigrams_untrunked.txt", ios::in);
	ifstream cypher("cypher.txt", ios::in);
	if(!ibfTokens){
		cout<<"there was an error opening the file"<<endl;
	}
	int nextNum;
	i = 0;
	while(ibfTokens.peek() != EOF){
		ibfTokens >> str;
		ibfTokens >> nextNum;
		for(int iv = i; iv < i+nextNum; iv++)
		{
			ibfToken[iv] = str;
		}
		i += nextNum;
	}
	
	i = 0;
	while(cypher.peek() != EOF){					
		cypher >> x;
		ibfCipher[i++] = x;
	}

	while(1)
	{
		for(int a = 0; a < k; a++)
		{
			ibfKey[a] = "*";
		}
		fill();
		printSolved();
		cout << endl;
	}

}


int main()
{
	IBF *ibf;
	ibf = new IBF;
	ibf->run();

	return 0;
}


