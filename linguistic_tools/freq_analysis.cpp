/*
	Tanner Oakes
	09.26.07
	
	CSCE 3210 Symbolic Processing
	
	Use to determine any character patterns found in a cipher.
	Hard coded for a maximum of 63 characters.
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_CHAR = 63;					// Maximum number of unique symbols in cipher
const char *MYFILE = "340cipher.txt";		// Given input file

int main()
{
	int freq[MAX_CHAR];								// Character frequency chart
	int pair[MAX_CHAR];								// Double letter chart
	int digram[MAX_CHAR][MAX_CHAR];					// Digram hit chart
	int trigram[MAX_CHAR][MAX_CHAR][MAX_CHAR];		// Trigram hit chart
	ifstream file(MYFILE);							// External file input
	int first;										// 1st symbol
	int second;										// 2nd symbol
	int third;										// 3rd symbol

	
	// Initialize arrays
	for (int i = 0; i < MAX_CHAR; i++)
	{
		freq[i] = 0;
		pair[i] = 0;
		for (int a = 0; a < MAX_CHAR; a++)
		{
			digram[i][a] = 0;
			for (int b = 0; b < MAX_CHAR; b++)
				trigram[i][a][b] = 0;
		}
	}
	
	// Scan for character frequency
	do
	{
		file >> first;
		freq[first] += 1;
	} while( !file.eof() );
	file.close();

	// Scan for digrams
	file.open(MYFILE);
	file >> first;
	do
	{
		file >> second;
		digram[first][second] += 1;
		if (first == second)
			pair[first] += 1;
		first = second;
	} while( !file.eof() );
	file.close();
	
	// Scan for trigrams
	file.open(MYFILE);
	file >> first;
	file >> second;
	do
	{
		file >> third;
		trigram[first][second][third] += 1;
		first = second;
		second = third;
	} while( !file.eof() );
	
	// Report character frequency
	cout << "CHARACTER FREQUENCY:" << endl;
	for (int i = 0; i < MAX_CHAR; i++)
		cout << i << " = " << freq[i] << " hits." << endl;
	
	// Scan digram chart and report any hits > 1
	cout << endl << endl << "DIGRAM ANALYSIS:" << endl;
	for (int i = 0; i < MAX_CHAR; i++)
		for (int a = 0; a < MAX_CHAR; a++)
			if (digram[i][a] > 1)
				cout << i << " " << a << " = " << digram[i][a] << " hits." << endl;
				
	// Scan for any double letters
	cout << endl << endl << "DOUBLE LETTERS:" << endl;
	for (int i = 0; i < MAX_CHAR; i++)
		if (pair[i] > 0)
			cout << i << " = " << pair[i] << " hits." << endl;
				
	// Scan trigram chart and report any hits > 1
	cout << endl << endl << "TRIGRAM ANALYSIS:" << endl;
	for (int i = 0; i < MAX_CHAR; i++)
		for (int a = 0; a < MAX_CHAR; a++)
			for (int b = 0; b < MAX_CHAR; b++)
				if (trigram[i][a][b] > 1)
					cout << i << " " << a << " " << b << " = " << trigram[i][a][b] << " hits." << endl;

	return 0;
}