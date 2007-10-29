Scorer::Scorer() {}

long Scorer::getScore()
{
	return score;
}

void Scorer::ReadNGraphs(bool LOGWEIGHT)
{
	string tempString;

	int count;
	
	double freq;

	//initialize n-graph arrays
	for(int i = 0; i < 676; i++)
		BI_GRAPHS[i] = 0;
	for(int i = 0; i < 17576; i++)
		TRI_GRAPHS[i] = 0;
	for(int i = 0; i < 456976; i++)
		TETRA_GRAPHS[i] = 0;
	for(int i = 0; i < 11881376; i++)
		PENTA_GRAPHS[i] = 0;

	//open n-graph files for reading
	ifstream biFile ("bigraphs.txt");
	if (!biFile) 
	{
		cerr<<"Couldn't open BI_GRAPHS.txt."<<endl;
		exit(1);
	}

	ifstream triFile ("trigraphs.txt");
	if (!triFile) 
	{
		cerr<<"Couldn't open TRI_GRAPHS.txt."<<endl;
		exit(1);
	}
	
	ifstream tetraFile ("tetragraphs.txt");
	if (!tetraFile) 
	{
		cerr<<"Couldn't open TETRA_GRAPHS.txt."<<endl;
		exit(1);
	}

	ifstream pentaFile ("pentagraphs.txt");
	if (!pentaFile) 
	{
		cerr<<"Couldn't open PENTA_GRAPHS.txt."<<endl;
		exit(1);
	}

	//read BI_GRAPHS and fill array
	while (!biFile.eof()) 
	{
		biFile>>tempString;
		biFile>>count;
		biFile>>freq;

		if(LOGWEIGHT)
			BI_GRAPHS[((tempString[0]-65) * 26) + (tempString[1]-65)] = (int)(log((double)count) * 10);
		else
			BI_GRAPHS[((tempString[0]-65) * 26) + (tempString[1]-65)] = (int)(freq * 100);
	}

	//read TRI_GRAPHS and fill array
	while (!triFile.eof()) 
	{
		triFile>>tempString;
		triFile>>count;
		triFile>>freq;

		if(LOGWEIGHT)
			TRI_GRAPHS[((tempString[0]-65) * 676) + ((tempString[1]-65) * 26) + (tempString[2]-65)] = (int)(log((double)count) * 10);
		else
			TRI_GRAPHS[((tempString[0]-65) * 676) + ((tempString[1]-65) * 26) + (tempString[2]-65)] = (int)(freq * 100);
	}

    //read TETRA_GRAPHS and fill array
	while (!tetraFile.eof()) 
	{
		tetraFile>>tempString;
		tetraFile>>count;
		tetraFile>>freq;

		if(LOGWEIGHT)
			TETRA_GRAPHS[((tempString[0]-65) * 17576) + ((tempString[1]-65) * 676) + ((tempString[2]-65) * 26) + (tempString[3]-65)] = (int)(log((double)count) * 10);
		else
			TETRA_GRAPHS[((tempString[0]-65) * 17576) + ((tempString[1]-65) * 676) + ((tempString[2]-65) * 26) + (tempString[3]-65)] = (int)(freq * 100);
	}

	//read PENTA_GRAPHS and fill array
	while (!pentaFile.eof()) 
	{
		pentaFile>>tempString;
		pentaFile>>count;
		pentaFile>>freq;

		if(LOGWEIGHT)
			PENTA_GRAPHS[((tempString[0]-65) * 456976) + ((tempString[1]-65) * 17576) + ((tempString[2]-65) * 676) + ((tempString[3]-65) * 26) + (tempString[4]-65)] = (int)(log((double)count) * 10);
		else
			PENTA_GRAPHS[((tempString[0]-65) * 456976) + ((tempString[1]-65) * 17576) + ((tempString[2]-65) * 676) + ((tempString[3]-65) * 26) + (tempString[4]-65)] = (int)(freq * 100);
	}
}

void Scorer::InitializeDictionary()
{
	for (int i = 0; i < 101; i++) 
	{								
	  Scores[i] = i;
	}

	Dictionary.FillDictionary(Dictionary, DICTIONARY_FILE, Scores, MAX_WORD_SCORE); 

	Dictionary.SetCustomWordScore(Dictionary, "enter word here", 99, Scores);
}

void Scorer::ScoreKey(string key)
{
	string word = "";
	string solution = "";

	float biScore;
	float triScore;
	float tetraScore;
	float pentaScore;

	float wordScore;
	float nGraphScore;
	float letterScore;

	float cipherFreqs[26] = {0};

	bool newBestScore = false;

	float totalScore = 0;

	for(int i = 0; i < NUM_KEYS; i++)
	{
		biScore = 0;
		triScore = 0;
		tetraScore = 0;
		pentaScore = 0;

		wordScore = 0;
		nGraphScore = 0;
		letterScore = 100;

		solution = keys[i].solution;

		for(int i = 0; i < solution.length(); i++)
			solution[i] = (char)(solution[i] + 32);

		for(int l = 0; l < 26; l++)
			cipherFreqs[l] = 0;
		
		if(keys[i].score == 0)
		{	
			for(int c = 0; c < CIPHER_SIZE; c++)
			{
				//score BI_GRAPHS
				if(c < CIPHER_SIZE - 1)
					biScore += BI_GRAPHS[((solution[c]-97) * 26) + (solution[c+1]-97)];
			
				//score TRI_GRAPHS
				if(c < CIPHER_SIZE - 2)
					triScore += TRI_GRAPHS[((solution[c]-97) * 676) + ((solution[c+1]-97) * 26) + (solution[c+2]-97)];

				//score TETRA_GRAPHS
				if(c < CIPHER_SIZE - 3)
					tetraScore += TETRA_GRAPHS[((solution[c]-97) * 17576) + ((solution[c+1]-97) * 676) + ((solution[c+2]-97) * 26) + (solution[c+3]-97)];

				//score PENTA_GRAPHS
				if(c < CIPHER_SIZE - 4)
					pentaScore += PENTA_GRAPHS[((solution[c]-97) * 456976) + ((solution[c+1]-97) * 17576) + ((solution[c+2]-97) * 676) +((solution[c+3]-97) * 26) + (solution[c+4]-97)];

				//score potential words
				word = solution.substr(c, 2);
					wordScore += Dictionary.GetWordScore(Dictionary, word);
				word = solution.substr(c, 3);
					wordScore += Dictionary.GetWordScore(Dictionary, word);
				word = solution.substr(c, 4);
					wordScore += Dictionary.GetWordScore(Dictionary, word);
				word = solution.substr(c, 5);
					wordScore += Dictionary.GetWordScore(Dictionary, word);
				word = solution.substr(c, 6);
					wordScore += Dictionary.GetWordScore(Dictionary, word);
				word = solution.substr(c, 7);
					wordScore += Dictionary.GetWordScore(Dictionary, word);
				word = solution.substr(c, 8);
					wordScore += Dictionary.GetWordScore(Dictionary, word);
				word = solution.substr(c, 9);
					wordScore += Dictionary.GetWordScore(Dictionary, word);
				word = solution.substr(c, 10);
					wordScore += Dictionary.GetWordScore(Dictionary, word);
				word = solution.substr(c, 11);
					wordScore += Dictionary.GetWordScore(Dictionary, word);
				word = solution.substr(c, 12);
					wordScore += Dictionary.GetWordScore(Dictionary, word);

				//score letters
				cipherFreqs[solution[c] - 97]++;
			}

			//calculate biScore
			if(biScore > 25345)
				biScore = 100;
			else
				biScore = (biScore / 25345) * 100;

			//calculate triScore
			if(triScore > 4690)
				triScore = 100;
			else
				triScore = (triScore / 4690) * 100;

			//calculate tetraScore
			if(tetraScore > 850)
				tetraScore = 100;
			else
				tetraScore = (tetraScore / 850) * 100;

			//calculate pentaScore
			if(pentaScore > 230)
				pentaScore = 100;
			else
				pentaScore = (pentaScore / 230) * 100;

			//calculate nGraphScore
			nGraphScore = (biScore * 0.25) + (triScore * 0.25) + (tetraScore * 0.25) + (pentaScore * 0.25);

			//calculate wordScore
			if(wordScore > 950)
				wordScore = 100;
			else
				wordScore = (wordScore / 950) * 100;

			//calculateLetterScore
			for(int l = 0; l < 26; l++)
			{
				cipherFreqs[l] = (cipherFreqs[l] / CIPHER_SIZE) * 100;

				letterScore = letterScore - fabs(letterFreqs[l] - cipherFreqs[l]);
			}

			//cout<<i<<": N-"<<nGraphScore<<" W-"<<wordScore<<" L-"<<letterScore<<endl<<endl;

			//calculate total score
			keys[i].score = (nGraphScore * 0.33333) + (wordScore * 0.33333) + (letterScore * 0.33333);
		}

		//cout<<i<<": N-"<<nGraphScore<<" W-"<<wordScore<<" L-"<<letterScore<<endl;
		
		if(keys[i].score > bestKey.score+1)
		{
			rePrint = true;

			bestKey = keys[i];
		}
	}
}

void Scorer::ReadFreqs(string FREQ_FILE)
{
	ifstream letterFile(FREQ_FILE);
	if(!letterFile)
	{
		cout<<endl<<"Could not open " << FREQ_FILE << " file."<<endl;
		cout<<"Press any key to exit..."<<endl;
		cin.get();
		return;
	}

	string temp;
	int letterIndex = 0;
	while (!letterFile.eof()) 
	{
		letterFile>>temp;
		letterFile>>letterFreqs[letterIndex];

		letterIndex++;
	}
}