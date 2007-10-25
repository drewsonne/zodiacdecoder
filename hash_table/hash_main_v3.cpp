/* 
 Use the function
   bool AddKey(std::string key, T* member)
 to add keys. Note the second parameter is an ADDRESS.

 To get an entry use 
     T* GetMember(const std::string& key)
 Note that it returns a POINTER to your data, not the data itself.

Check out hash_table.h for more documentation and the list of functions available.
 
 "hash_table.h" is the only file you need to include to use
 the hash table, BUT you need to HAVE all three header files:
 "hash_fun.h", "hash_table.h", and "Stl.h"
 in the folder I included for it to work.

 - Daniel 
 johndleon@gmail.com
 281-814-5482
*/

#include "include\hash_table.h"
#include <iostream>
#include <fstream>


void main() {

	// INITIALIZATIONS
	std::ifstream InFile("words.txt");									// text file containing dictionary words
	std::string TempWord;
	int Scores[101]; 
	for (int i = 0; i < 101; i++) {										// Initialize Scores array
	  Scores[i] = i;
	}
	CHashTable<int> Dictionary;							
	


	// FILL DICTIONARY
		while (InFile >> TempWord) {
			std::cout << TempWord;
			if (Dictionary.GetMember(TempWord) == NULL) {							// Only if word is not already in dictionary
				Dictionary.AddKey(TempWord, &Scores[TempWord.length()]);	// Score is equal to word length
			}
		}
	// END FILL DICTIONARY



	// SET CUSTOM WORD SCORE
		/* If we want to set a custom score for words, do it here, after the dictionary is already full
		   I haven't made this into a function yet, but this is the idea
		
		std::string	CustomWord = "MyWord";
		int CustomScore = 25;
		if (Dictionary.GetMember(CustomWord))						// Remove key if it already exists
			Dictionary.Remove(CustomWord, false);
		Dictionary.AddKey(CustomWord, &Scores[CustomScore]);
		*/
	// END SET CUSTOM WORD SCORE



	// GET WORD SCORE
		int * ScorePointer;
		int Score;

		ScorePointer = Dictionary.GetMember("asank");
		
		if (ScorePointer != NULL)						// If that word was found
			Score = *ScorePointer;
		else
			Score = 0;
		std::cout << Score << std::endl;				// Output the score of the word
	// END GET WORD SCORE


	Dictionary.RemoveAllKey(false);										/* clean up the hash (free memory) */
}
