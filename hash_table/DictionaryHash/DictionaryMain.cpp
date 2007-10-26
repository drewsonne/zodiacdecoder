/* 
 Daniel 
 johndleon@gmail.com
 281-814-5482
*/

// DICTIONARY DEFINITIONS
#define DICTIONARY_FILE "words.txt"
#define MAX_WORD_SCORE 1000

#include "include\hash_table.h"
#include <iostream>
#include <fstream>



void main() {
	// INITIALIZATIONS
	CHashTable<int> Dictionary;
	int Scores[MAX_WORD_SCORE + 1];			// SCORES CAN BE MADE GLOBAL
	for (int i = 0; i < 101; i++) {			// Initialize Scores array, cannot be in globalspace
	  Scores[i] = i;
	}


	// FillDictionary returns true on success
	Dictionary.FillDictionary(Dictionary, DICTIONARY_FILE, Scores, MAX_WORD_SCORE); 


	// SetCustomWordScore returns true on success
	Dictionary.SetCustomWordScore(Dictionary, "CustomWord", 25, Scores); // Returns true on success


	// GetWordScore returns 0 if word does not exist
	std::cout << Dictionary.GetWordScore(Dictionary, "CustomWord");

	//Clean up at end -- not necessary but polite
	Dictionary.RemoveAllKey(false);										/* clean up the hash) */
}
