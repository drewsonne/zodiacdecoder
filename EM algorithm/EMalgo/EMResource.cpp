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

	xmlElement->Attribute("count", &totalDigrams);

	for(childElement = xmlElement->FirstChildElement("Digram"); childElement; childElement = childElement->NextSiblingElement("Digram")) {

		childElement->Attribute("first", &firstAlpha);
		childElement->Attribute("second", &secondAlpha);
		childElement->Attribute("count", &digramCount);
		if (firstAlpha >= 0 && firstAlpha < kMaxAlphaChar && secondAlpha >= 0 && secondAlpha < kMaxAlphaChar) {
			double prob = (double) digramCount / (double) totalDigrams;
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

// display the solution table - mostly for debugging.

void displaySolutionTable(const SSolutionTable &st) {

	for (int j = 1; j < kMaxCipherChar; j++) {
		cout << "r1:\t";   // change to 'A' or first symbol depending on row major, etc.
		for (int i = 0; i < kMaxAlphaChar; i++) {
			cout << st.prob[j][i] << "\t";
		}
		cout << endl;
	}

}