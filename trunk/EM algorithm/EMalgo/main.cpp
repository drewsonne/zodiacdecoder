/*

 An EM Algorithm for deciphering the Zodiac-340 cipher
 Algorithm: Kevin Knight from "Decoding Complexity in Word-Replacement Translation Models"
 Most Code: Harold Myles
 Some Code: Ryan Garlick

*/

/*

Problems (Search for Problem1, Problem2, ... in the code)
---------------------------------------------------------

Problem1: At the bottom of p.2. in "Decoding Complexity..." it reads "once the s(f|e) table
has been learned, there is a similar O(mv^2) algorithm for optimal decoding. Not sure the best way to extract the 
letter from each symbol

Problem2: The Q and R tables converge to zero in the second EM iteration from the multiplications
in the inner-most for loops in the forward and reverse pass. Setting the kInitSTStartValue in EMConsts to 1.0 preserves values through
the first EM iteration only. Setting kInitSTStartValue to 1/26 as in the paper makes the values in the Q and R tables
go to zero.

Problem3: Uncertainty about how to properly implement the last line in the algorithm:
"Normalize c(f|e) table to create a revised s(f|e)"

Knight comments:

> Problem1: At the bottom of p.2. in "Decoding Complexity..." it reads
> "once the s(f|e) table
> has been learned, there is a similar O(mv^2) algorithm for optimal
> decoding. Not sure the best way to extract the
> letter from each symbol
this is also known as the viterbi algorithm
> Problem2: The Q and R tables converge to zero in the second EM
> iteration from the multiplications
> in the inner-most for loops in the forward and reverse pass. Setting
> the kInitSTStartValue in EMConsts to 1.0 preserves values through
> the first EM iteration only. Setting kInitSTStartValue to 1/26 as in
> the paper makes the values in the Q and R tables
> go to zero.
not sure why tables go to zero -- there should always be some (low)
fractional count for <cipher-letter, plaintext-Q> or <cipher-Q,
plaintext-letter>.
> Problem3: Uncertainty about how to properly implement the last line in
> the algorithm:
> "Normalize c(f|e) table to create a revised s(f|e)"
for each ei,
total = sum over all fj of c(fj | ei)
s(fj | ei) = c(fj | ei) / total

*/


#include <iostream>
#include <fstream>
#include <string>
#include "tinyxml.h"
#include "EMconsts.h"
#include <vector>
#include <cctype>

using namespace std;

struct SDigram {
	int first;
	int second;
};

struct SBModel{					// struct for holding the source model (the bigram percentages)
	double model[kMaxAlphaChar + 1][kMaxAlphaChar + 1];
};

struct SCipher{
	SCipher() {
		cipherText = NULL;
		size = 0;
	}
	~SCipher() {
		if (cipherText) {
			delete[] cipherText;
		}
	}
	int size;
	int *cipherText;
};

struct SKey {
	int cipherToAlphaMap[kMaxCipherChar];
};

struct SSolutionTable {

	void multi(double coefficient) {
		for (int i = 0; i < kMaxAlphaChar; i++) {
			for (int k = 0; k < kMaxCipherChar; k++) {
				prob[i][k] *= coefficient;
			}
		}
	}
	void zero() {
		for (int i = 0; i < kMaxAlphaChar; i++) {
			for (int k = 0; k < kMaxCipherChar; k++) {
				prob[i][k] = 0.0f;
			}
		}
	}

	void init(const double &initialValue) {
		for (int i = 0; i < kMaxAlphaChar; i++) {
			for (int k = 0; k < kMaxCipherChar; k++) {
				prob[i][k] = initialValue;
			}
		}
	}

	// The most naive way of chosing a key from the table.
	// Don't use this. Problem1
	void getHighestProbableKeyPerColumn(SKey &key) {

		for (int i = 0; i < kMaxCipherChar; i++) {
			double highest = 0.0f;
			int highestIndex = -1;
			for (int k = 0; k < kMaxAlphaChar; k++) {
				if (prob[k][i] > highest) {
					highest = prob[k][i];
					highestIndex = k;
				}
			}

			if (highestIndex >= 0) {
				key.cipherToAlphaMap[i] = highestIndex;
			} 
		}
	}


	// This method isn't too good either.  We not only force every alpha character to be mapped, but mapped twice.
	// There is no good assumption that all alpha characters will be represented in the cipher. Does the 340 cipher have
	// a letter 'Q'? Problem1

	bool getHighestProbableKey(SKey &key) {
		//First we go through each alpha character and chose the map it to the column with highest prob.
		bool columnIsChosen[kMaxCipherChar];
		bool rowIsChosen[kMaxAlphaChar];
		for (int i = 0; i <kMaxCipherChar; i++) {
			columnIsChosen[i] = false;
		}

		for (int i = 0; i <kMaxAlphaChar; i++) {
			rowIsChosen[i] = false;
		}

		int rowsChosen = 0;
		int columnsChosen = 0;

		while (rowsChosen < kMaxAlphaChar) {
			double highest= -1.0;
			int column;
			int row;
			for (int i = 0; i <kMaxAlphaChar; i++) {
				for (int k = 0; k <kMaxCipherChar; k++) {
					if (prob[i][k] > highest && !columnIsChosen[k] && !rowIsChosen[i]) {
						highest = prob[i][k];
						column = k;
						row = i;
					}
				}
			}

			if (highest < 0.0) {
				return false;
			}

			key.cipherToAlphaMap[column] = row;
			columnIsChosen[column] = true;
			rowIsChosen[row] = true;
			++rowsChosen;
			++columnsChosen;
		}

		rowsChosen = 0;
		for (int i = 0; i <kMaxAlphaChar; i++) {
			rowIsChosen[i] = false;
		}

		while (rowsChosen < kMaxAlphaChar) {
			double highest= -1.0;
			int column;
			int row;
			for (int i = 0; i <kMaxAlphaChar; i++) {
				for (int k = 0; k <kMaxCipherChar; k++) {
					if (prob[i][k] > highest && !columnIsChosen[k] && !rowIsChosen[i]) {
						highest = prob[i][k];
						column = k;
						row = i;
					}
				}
			}

			if (highest < 0.0) {
				return false;
			}

			key.cipherToAlphaMap[column] = row;
			columnIsChosen[column] = true;
			rowIsChosen[row] = true;
			++rowsChosen;
			++columnsChosen;
		}

		rowsChosen = 0;
		for (int i = 0; i <kMaxAlphaChar; i++) {
			rowIsChosen[i] = false;
		}

		while (columnsChosen < kMaxCipherChar) {
			double highest= -1.0;
			int column;
			int row;
			for (int i = 0; i <kMaxAlphaChar; i++) {
				for (int k = 0; k <kMaxCipherChar; k++) {
					if (prob[i][k] > highest && !columnIsChosen[k] && !rowIsChosen[i]) {
						highest = prob[i][k];
						column = k;
						row = i;
					}
				}
			}

			if (highest < 0.0) {
				return false;
			}

			key.cipherToAlphaMap[column] = row;
			columnIsChosen[column] = true;
			rowIsChosen[row] = true;
			++rowsChosen;
			++columnsChosen;
		}

		return true;
	}

	void display() {

		for (int i = 0; i < kMaxAlphaChar-20; i++) {
			cout << "r" << i << ":\t";
			for (int k = 0; k < kMaxCipherChar-55; k++) {
				cout << prob[i][k] << "\t";
			}
			cout << endl;
		}
	}

	double prob[kMaxAlphaChar][kMaxCipherChar];
};

bool EMalgo (const SBModel &model, const SCipher &cipher, SSolutionTable &initialSolutionTable, SSolutionTable &returnSolutionTable) {
	
	// We need two arrays to represent possible mappings of alpha characters into the cipher text.
	double *F[kMaxAlphaChar];
	double *R[kMaxAlphaChar];
	for (int i = 0; i < kMaxAlphaChar; i++) {
		F[i] = new double[cipher.size];
		if (F[i] == NULL) {
			return false;
		}
		R[i] = new double[cipher.size];
		if (R[i] == NULL) {
			return false;
		}
	}
	
	for (int iter = 0; iter < kEMiterations; iter++) {  // begin main EM loop

		// We set the possible solution to zero.
		returnSolutionTable.zero();

		// Now We begin the foward pass
		// First set the beginning character to boundary probabilites
		for (int i = 0; i < kMaxAlphaChar; i++) {
			F[i][0] = model.model[kBoundaryChar][i];
		}

		// We look at each character in the cipher. Starting with the second
		for (int j = 1; j < cipher.size; j++) {
			// And we look at all possible digrams
			for (int i = 0; i < kMaxAlphaChar; i++) {
				F[i][j] = 0.0f;
				for (int k = 0; k < kMaxAlphaChar; k++) {
					// Problem2
					F[i][j] += F[k][j-1] * model.model[k][i] * initialSolutionTable.prob[k][cipher.cipherText[j-1]];
				}
			}
		}

		// Then we begin the reverse pass
		// Again first we set the last character to boundary probabilites
		for (int i = 0; i < kMaxAlphaChar; i++) {
			R[i][cipher.size-1] = model.model[i][kBoundaryChar];
		}

		// We look at each character in the cipher. Starting with the next to the last
		for (int a = cipher.size-2; a >= 0; a--) {
			// And we look at all possible digrams
			for (int b = 0; b < kMaxAlphaChar; b++) {
				R[b][a] = 0.0f;
				for (int c = 0; c < kMaxAlphaChar; c++) {
					// Problem2
					R[b][a] += R[c][a+1] * model.model[b][c] * initialSolutionTable.prob[c][cipher.cipherText[a+1]];
				}
			}
		}
     
		// Finally we build and normalize our final solution for this iteration
		for (int j = 0; j < cipher.size; j++) {
			for (int i = 0; i < kMaxAlphaChar; i++) {
				returnSolutionTable.prob[i][cipher.cipherText[j]] += F[i][j] * R[i][j] * initialSolutionTable.prob[i][cipher.cipherText[j]];
			}
		}

	/* Harold original normalization code
	double maxValue = -1.0f;
	for (int j = 0; j < kMaxCipherChar; j++) {
		for (int i = 0; i < kMaxAlphaChar; i++) {
			if (returnSolutionTable.prob[i][j] <= 0.0) {				return false;			}
			if (maxValue <= returnSolutionTable.prob[i][j]) {				maxValue = returnSolutionTable.prob[i][j];			}
		}
	}
	for (int j = 0; j < kMaxCipherChar; j++) {
		for (int i = 0; i < kMaxAlphaChar; i++) {			returnSolutionTable.prob[i][j] = returnSolutionTable.prob[i][j] / maxValue;		}
	}
    */   // end Harold original normalization code

    // new normalization routine - normalize with Lagrangian smoothing (ensure no zero entries).
	
		double totalProbs = 0.0f;
		for (int i = 0; i < kMaxCipherChar; i++) {
			for (int j = 0; j < kMaxAlphaChar; j++) {
				if (returnSolutionTable.prob[j][i] <= 0.0) {  // a little error check
					return false;
				}
				totalProbs += returnSolutionTable.prob[j][i]; // let's sum the values for this letter (a-z)
			}
			for (int j = 0; j < kMaxAlphaChar; j++) {
	         returnSolutionTable.prob[j][i] = (returnSolutionTable.prob[j][i] + kLagrangeValue) / (totalProbs + ( kMaxAlphaChar * kLagrangeValue ) );
			}
		}
	
		// Problem3: use returnSolutionTable to modify initSolutionTable
		for (int i = 0; i < kMaxCipherChar; i++) {
			for (int j = 0; j < kMaxAlphaChar; j++) {
				initialSolutionTable.prob[j][i] *= returnSolutionTable.prob[j][i];
			}
		}


	}  // end of main EM loop to kEMiterations

	// Clean up 
	for (int i = 0; i < kMaxAlphaChar; i++) {
		if (F[i] != NULL) {
			delete[] F[i];
		}
		if (R[i] != NULL) {
			delete[] R[i];
		}
	}
	return true;
}

bool readModelerFile(const string &fileName, SBModel &model, string &errors) {

	// First Initialize the model

	for (int i = 0; i <= kMaxAlphaChar; i++) {
		for (int k = 0; k <= kMaxAlphaChar; k++) {
			model.model[i][k] = -1.0f;
		}
	}

	TiXmlDocument xmlDoc(fileName.c_str());
	if (!xmlDoc.LoadFile()) {
		errors += ":readModelerFile: cannot read xml file " + fileName;
		return false;
	}

	// First read in total boundaries
	TiXmlElement * xmlElement = NULL;

	if (!(xmlElement = xmlDoc.FirstChildElement("Boundaries"))) {
		errors += ":readModelerFile: xml file " + fileName + " corrupt.";
		return false;
	}

	int totalBoundary;
	xmlElement->Attribute("count", &totalBoundary);

	TiXmlElement * childElement = NULL;
	int alphaChar = 0;
	int startCount = 0;
	int endCount = 0;
	for(childElement = xmlElement->FirstChildElement("Boundary"); childElement; childElement = childElement->NextSiblingElement("Boundary")) {

		childElement->Attribute("value", &alphaChar);
		childElement->Attribute("startCount", &startCount);
		childElement->Attribute("endCount", &endCount);
		if (alphaChar >= 0 && alphaChar < kMaxAlphaChar) {
			double prob = (double) endCount / (double) totalBoundary;
			model.model[alphaChar][kBoundaryChar] = prob;
			prob = (double) startCount / (double) totalBoundary;
			model.model[kBoundaryChar][alphaChar] = prob;
		} else {
			errors += ":readModelerFile: invalid boundary character.";
			return false;
		}
	}

	// Read in digrams
	xmlElement = NULL;

	if (!(xmlElement = xmlDoc.FirstChildElement("Digrams"))) {
		errors += ":readModelerFile: xml file " + fileName + " corrupt.";
		return false;
	}

	childElement = NULL;
	int firstAlpha, secondAlpha, digramCount, totalDigrams;
	double firstCharTotal[kMaxAlphaChar];

	for (int i = 0; i < kMaxAlphaChar; i++) {
		firstCharTotal[i] = 0.0f;
	}

	xmlElement->Attribute("count", &totalDigrams);

	// first, get the totals for each 'first' character - added by RG
	for(childElement = xmlElement->FirstChildElement("Digram"); childElement; childElement = childElement->NextSiblingElement("Digram")) {

		childElement->Attribute("first", &firstAlpha);
		childElement->Attribute("second", &secondAlpha);
		childElement->Attribute("count", &digramCount);
		if (firstAlpha >= 0 && firstAlpha < kMaxAlphaChar && secondAlpha >= 0 && secondAlpha < kMaxAlphaChar) {
			firstCharTotal[firstAlpha] += (double)digramCount; 
		} else {
			errors += ":readModelerFile: invalid digram character.";
			return false;
		}
	}

	for(childElement = xmlElement->FirstChildElement("Digram"); childElement; childElement = childElement->NextSiblingElement("Digram")) {

		childElement->Attribute("first", &firstAlpha);
		childElement->Attribute("second", &secondAlpha);
		childElement->Attribute("count", &digramCount);
		if (firstAlpha >= 0 && firstAlpha < kMaxAlphaChar && secondAlpha >= 0 && secondAlpha < kMaxAlphaChar) {
			double prob = (double) digramCount / firstCharTotal[firstAlpha];		// new - RG calculating percentages over each first character.
//			double prob = (double) digramCount / (double) totalDigrams;     ORIGINAL calculating percentage over all digrams 
			model.model[firstAlpha][secondAlpha] = prob;
		} else {
			errors += ":readModelerFile: invalid digram character.";
			return false;
		}
	}

	int toleranceCount = 0;
	for (int i = 0; i <= kMaxAlphaChar; i++) {
		for (int k = 0; k <= kMaxAlphaChar; k++) {
			if (model.model[i][k] < kMinTolerance) {
				model.model[i][k] = kMinTolerance;
				++toleranceCount;
			}
		}
	}

	cout << endl << "TOLERANCE HIT: " << toleranceCount << endl;
	return true;
}

bool readConfigFile(const string &fileName, string &modelFilename, string &cipherFilename, string &errors) {
	TiXmlDocument xmlDoc(fileName.c_str());
	if (!xmlDoc.LoadFile()) {
		errors += ":ReadConfigFile: cannot read xml file " + fileName;
		return false;
	}

	TiXmlElement * xmlElement = NULL;

	if (!(xmlElement = xmlDoc.FirstChildElement("ModelFile"))) {
		errors += ":ReadConfigFile: xml file " + fileName + " has no ModelFile.";
		return false;
	}

	modelFilename = xmlElement->Attribute("filename");

	xmlElement = NULL;

	if (!(xmlElement = xmlDoc.FirstChildElement("CipherFile"))) {
		errors += ":ReadConfigFile: xml file " + fileName + " has no CipherFile.";
		return false;
	}

	cipherFilename = xmlElement->Attribute("filename");

	return true;
}

bool readCipherFile(const string &fileName, SCipher &cipher, string &errors) {

	ifstream inFile(fileName.c_str(), ios::in);
	if (!inFile.is_open()) {
		errors += ":readCipherFile: cannot open cipher file " + fileName;
		return false;
	}

	inFile >> cipher.size;
	if (cipher.size <= 0) {
		errors += ":readCipherFile: invalid cipher file size" + fileName;
		return false;
	}

	cipher.cipherText = new int[cipher.size];
	int readCount = 0;

	while(!inFile.eof()) {
		if (readCount >= cipher.size) {
			errors += ":readCipherFile: cipher text longer than registered size";
			inFile.close();
			return false;
		}
		int temp;
		inFile >> temp;
		if (temp < 0 || temp >= kMaxCipherChar) {
			errors += ":readCipherFile: invalid cipher characters found.";
			inFile.close();
			return false;
		} 
		cipher.cipherText[readCount] = temp;
		++readCount;
	}

	if (readCount != cipher.size) {
			errors += ":readCipherFile: cipher size mismatch.";
			inFile.close();
			return false;
	}

	inFile.close();
	return true;
}
bool displayError(const string &errors) {
	cout << endl << "ERROR: " << errors << endl;
	cin.get();
	exit(1);
	return false;
}


void main() {
	string errors;
	string modelFilename, cipherFilename;
	if (!readConfigFile("config.xml", modelFilename, cipherFilename, errors)) {
		displayError(errors);	
	}

	SBModel bModel;
	if (!readModelerFile(modelFilename, bModel, errors)) {
		displayError(errors);
	}
	SCipher cipher;
	if (!readCipherFile(cipherFilename, cipher, errors)) {
		displayError(errors);
	}

	// Let's run the EM algorithm
	SSolutionTable initialST, returnST;
	initialST.init(kInitSTStartValue);
	if (!EMalgo(bModel, cipher, initialST, returnST)) {
		displayError("EMalgo function returned an error condition.");
	}

	// Create a key and write it to file.
	SKey key;

	if (getHighestProbableKeyPerColumn) {
		initialST.getHighestProbableKeyPerColumn(key);
	}
	else {
		if (!initialST.getHighestProbableKey(key)) {
			cout << endl << "Not all Alpha chars represented in key" << endl;
		}
	}
	

	ofstream outFile("key.out", ios::out | ios::trunc);
	if (!outFile.is_open()) {
		displayError("Cannot open Output file");
	}

	for (int i = 0; i < kMaxCipherChar; i++) {
		outFile << key.cipherToAlphaMap[i] << endl;
	}
	outFile.close();
	cin.get();
}



