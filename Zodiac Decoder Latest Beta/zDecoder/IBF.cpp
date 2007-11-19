#include "stdafx.h"
#include "IBF.h"

#define MAX_WORD_SCORE 1000

using namespace std;

const unsigned long t = 4897368; //size of ibfToken library
const unsigned short c = 340; //size of cypher
const unsigned short k = 63; //size of ibfKey
const unsigned short n = 2; //size of ibfTokens/n-grams

int ibfCipher[c];

double letterFreqs[26] = {0};

string ibfToken[t];
string ibfKey[k];
string ibftemp="";

IBF::IBF(string *keystream):Solver(keystream)
{
	string str;
	int i =0;
	int x;
	ifstream ibfTokens("ibf_bigrams.txt", ios::in);
	ifstream cypher("cypher.txt", ios::in);
	if(!ibfTokens){
		cout<<"there was an error opening the file"<<endl;
		exit(0);
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
	srand((unsigned)time(0));
	sm=rand();
}

bool IBF::isFull(){
	for (int i = 0; i < 63; i++)
		if (ibfKey[i] == "*")
			return false;
	return true;
}

void IBF::printibfKey() 
{
	ibftemp.clear();
	for (int i = 0; i < 63; i++)
		ibftemp+=ibfKey[i];
	*KeyStream=ibftemp;
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
	int count = 0;

	for(int z = 0; z < 63; z++)
		temp[z]=ibfKey[z];

	for(p = 0; p < n; p++)
	{
		if ( (ibfKey[ibfCipher[position + p]] != "*") && ((ibfKey[ibfCipher[position+p]]).at(0) != (ibfToken[token]).at(p)) )
		{
			return false;
		}
		else
		{
			if(ibfKey[ibfCipher[position + p]] != "*")
				count++;

			temp[ibfCipher[position+p]] = (ibfToken[token]).at(p);
		}

		if(count == n)
			return false;
	}

	for(int z = 0; z < 63; z++)
		ibfKey[z]=temp[z];
	return true;
}


unsigned long IBF::fill(unsigned long seedModifier){
    string str;
    int i;
    bool placed;
    unsigned long randomT;
	const int x = c - (n - 1); //x = c - (n - 1) 

	while(! isFull() )
	{
 		randomT = (unsigned long)(rand() * rand() * rand() * seedModifier) % t;

		placed = false;
		i = 0;
		while ( (!placed) && (i < x) )
		{
			placed = fit(randomT, i);
			i++;
		}
	}

	return randomT;
}


void IBF::run(){

	for(int a = 0; a < k; a++)
	{
		ibfKey[a] = "*";
	}
	sm = fill(sm);
	printibfKey();

}


//int main()
//{
//	IBF *ibf;
//	ibf = new IBF;
//	ibf->run();
//}