#include <iostream>
#include <fstream>
#include <string>
#include "tinyxml.h"
#include <vector>
#include <cctype>

using namespace std;

#define ALPHA_TOKEN_COUNT 26

struct SEMModel {

	SEMModel() {
		totalBoundaries = 0;
		totalDigrams = 0;
		totalTokens = 0;
		for (int i = 0; i < ALPHA_TOKEN_COUNT; i++) {
			startBoundaries[i] = 0;
			endBoundaries[i] = 0;
			tokens[i] = 0;
			for (int k = 0; k < ALPHA_TOKEN_COUNT; k++) {
				digrams[i][k] = 0;
			}
		}
	}

	void addBoundary(char tokenStart, char tokenEnd) {
		tokenStart = tokenStart - 'A';
		tokenEnd = tokenEnd - 'A';
		if (tokenStart >= 0 && tokenStart < ALPHA_TOKEN_COUNT && tokenEnd >= 0 && tokenEnd < ALPHA_TOKEN_COUNT ) {
			startBoundaries[tokenStart]++;
			endBoundaries[tokenEnd]++;
			totalBoundaries++;
		}	
	}

	void addDigram(char tokenStart, char tokenEnd) {
		tokenStart = tokenStart - 'A';
		tokenEnd = tokenEnd - 'A';
		if (tokenStart >= 0 && tokenStart < ALPHA_TOKEN_COUNT && tokenEnd >= 0 && tokenEnd < ALPHA_TOKEN_COUNT ) {
			digrams[tokenStart][tokenEnd]++;
			totalDigrams++;
		}	
	}

	void addToken(char token) {
		token = token - 'A';
		if (token >= 0 && token < ALPHA_TOKEN_COUNT ) {
			tokens[token]++;
			totalTokens++;
		}
	}
	int totalBoundaries;
	int totalTokens;
	int tokens[ALPHA_TOKEN_COUNT];
	int startBoundaries[ALPHA_TOKEN_COUNT] ;
	int endBoundaries[ALPHA_TOKEN_COUNT] ;
	int totalDigrams;
	int digrams[ALPHA_TOKEN_COUNT][ALPHA_TOKEN_COUNT] ;
};

bool ReadConfigFile(const string fileName, vector<string> &fileNames, string &errors) {

	TiXmlDocument xmlDoc(fileName.c_str());
	if (!xmlDoc.LoadFile()) {
		errors += ":ReadConfigFile: cannot read xml file " + fileName;
		return false;
	}

	TiXmlElement * xmlElement = NULL;

	if (!(xmlElement = xmlDoc.FirstChildElement("filelist"))) {
		errors += ":ReadConfigFile: xml file " + fileName + " has no filelist.";
		return false;
	}

	TiXmlElement * fileElement = NULL;
	int fileCount = 0;
	for (fileElement = xmlElement->FirstChildElement("file"); fileElement != NULL; fileElement = fileElement->NextSiblingElement()) {
		string temp = fileElement->Attribute("name");
		fileNames.push_back(temp);
		++fileCount;
	}

	if (fileCount <= 0) {
		errors += ":ReadConfigFile: xml file " + fileName + " has no filelist.";
		return false;
	}

	return true;
}

bool AnalyzeFile(string fileName, SEMModel &model, string &errors) {

	ifstream inFile;
	inFile.open(fileName.c_str(), ios::in);
	if(!inFile.is_open()) {
		errors += ":AnalyzeFile: cannot open file " + fileName;
		return false;
	}

	cout << endl << "Analyzing file: " << fileName;

	char *buffer = NULL;
	// get length of file:
	inFile.seekg (0, ios::end);
	int length = inFile.tellg();
	inFile.seekg (0, ios::beg);

	if (length > 0) {
		buffer = new char[length];
		if (buffer != NULL) {
	
			inFile.read(buffer, length);

			unsigned char currentChar, lastChar, startBoundary, endBoundary;

			currentChar = 0;
			lastChar = 0;
			startBoundary = 0;
			endBoundary = 0;

			bool lastCharIsAlpha = false;

			for (int i = 0; i < length; i++) {
				currentChar = buffer[i];
				if (isalpha(currentChar)) {
					currentChar = toupper(currentChar);

					if (!lastCharIsAlpha) {
						startBoundary = currentChar;
						if (endBoundary != 0) {
							model.addDigram(endBoundary, startBoundary);
						}
					} else {
						model.addDigram(lastChar, currentChar);
					}
					
					lastChar = currentChar;
					lastCharIsAlpha = true;

					model.addToken(currentChar);
				} else if (lastCharIsAlpha) {
					endBoundary = lastChar;
					model.addBoundary(startBoundary, endBoundary);
					lastCharIsAlpha = false;
				}
			}

			if (buffer) {
				delete[] buffer;
				buffer = NULL;
			}

		} else {
			errors += ":AnalyzeFile: cannot allocate file buffer.";
			return false;
		}
	}
	inFile.close();
	return true;
}

bool WriteReport(string fileName, const SEMModel &model, string &errors) {
	ofstream outFile;

	outFile.open(fileName.c_str(), ios::out | ios::trunc);

	if (!outFile.is_open()) {
		errors += ":WriteReport: cannot open output file " + fileName;
		return false;
	}

	outFile << model.totalTokens << " " << model.totalBoundaries << " " << model.totalDigrams << endl;

	for (int i = 0; i < ALPHA_TOKEN_COUNT; i++) {
		outFile << i << " " << model.tokens[i] << " " << model.startBoundaries[i] << " " << model.endBoundaries[i] << endl;
	}

	for (int i = 0; i < ALPHA_TOKEN_COUNT; i++) {
		for (int k = 0; k < ALPHA_TOKEN_COUNT; k++) {
			outFile << i << " " << k << " " << model.digrams[i][k] << endl;
		}
	}

	outFile.close();

	TiXmlDocument xmlDoc;
	fileName += ".xml";
	TiXmlElement tokenNode("Tokens");
	tokenNode.SetAttribute("count", model.totalTokens);
	for (int i = 0; i < ALPHA_TOKEN_COUNT; i++) {
		TiXmlElement childNode("Token");
		childNode.SetAttribute("value", i);
		childNode.SetAttribute("count", model.tokens[i]);
		tokenNode.InsertEndChild(childNode);
	}
	xmlDoc.InsertEndChild(tokenNode);

	TiXmlElement boundaryNode("Boundaries");
	boundaryNode.SetAttribute("count", model.totalBoundaries);

	for (int i = 0; i < ALPHA_TOKEN_COUNT; i++) {
		TiXmlElement childNode("Boundary");
		childNode.SetAttribute("value", i);
		childNode.SetAttribute("startCount", model.startBoundaries[i]);
		childNode.SetAttribute("endCount", model.endBoundaries[i]);
		boundaryNode.InsertEndChild(childNode);
	}
	xmlDoc.InsertEndChild(boundaryNode);

	TiXmlElement digramNode("Digrams");
	digramNode.SetAttribute("count", model.totalDigrams);
	for (int i = 0; i < ALPHA_TOKEN_COUNT; i++) {
		for (int k = 0; k < ALPHA_TOKEN_COUNT; k++) {
			TiXmlElement childNode("Digram");
			childNode.SetAttribute("first", i);
			childNode.SetAttribute("second", k);
			childNode.SetAttribute("count", model.digrams[i][k]);
			digramNode.InsertEndChild(childNode);
		}
	}
	xmlDoc.InsertEndChild(digramNode);

	xmlDoc.SaveFile(fileName.c_str());
	return true;
}
void main() {

	string errors;

	vector<string> fileList;
	if (!ReadConfigFile("emconfig.xml",fileList,errors)) {
		cout << endl << "ERROR:" << errors << endl;
		cin.get();
		return;
	}

	SEMModel theModel;

	vector<string>::iterator it = fileList.begin();
	while(it != fileList.end()) {
		if(!AnalyzeFile((*it), theModel, errors)) {
			cout << endl << "ERROR:" << errors << endl;
			cin.get();
			return;
		}
		++it;
	}

	if (!WriteReport("model.report", theModel, errors)) {
		cout << endl << "ERROR:" << errors << endl;
		cin.get();
		return;
	}

	cout << endl << "TOKENS: " << theModel.totalTokens << " BOUNDARIES: " << theModel.totalBoundaries << " DIGRAMS: " << theModel.totalDigrams<< endl;
	cin.get();
}