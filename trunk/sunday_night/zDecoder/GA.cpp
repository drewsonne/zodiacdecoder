 // GA.cpp - MAIN FILE FOR OUR ZODIAC 340 GENETIC ALGORITHM CLIENT

/////
// INCLUDES
/////

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include "include\hash_table.h"
#include "GA.h"

using namespace std;

GA::GA()
{
		bool rePrint = false;
		bool seedKeys = false;
		string seed = "ILIKEKILLINGBECAUSEIAMINSECUREABOUTMYSELFHAHEVALEJOANDOTHERCHAR";
		double letterFreqs[26] = {0};
}

//int main()
//{
//	cout <<"RUNNING"<<endl;
//
//	GA *ga;
//	ga = new GA;
//
//	cout <<"RUNNING"<<endl;
//	ga->run();
//
//	return 0;
//}

void GA::run()
{
	/////
	// SIMULATION PARAMETERS - SET/CHANGE DEFAULT VALUES IN DEFAULTS() FUNCTION
	/////
	int CIPHER_SIZE;
	int CIPHER_WIDTH;
	int CIPHER_HEIGHT;
	int KEY_SIZE;
	int NUM_KEYS;
	int GENERATIONS;

	bool LOGWEIGHT;

	string CIPHER_FILE;

	double BI_BONUS;
	double TRI_BONUS;
	double TETRA_BONUS;
	double PENTA_BONUS;

	double SURVIVAL_RATE;
	double BREED_RATE;
	double MUTATE_RATE;
	/////

	int selection;

	Defaults(CIPHER_SIZE, CIPHER_WIDTH, CIPHER_HEIGHT, KEY_SIZE, NUM_KEYS, GENERATIONS, LOGWEIGHT, CIPHER_FILE, BI_BONUS, TRI_BONUS, TETRA_BONUS, PENTA_BONUS, SURVIVAL_RATE, BREED_RATE, MUTATE_RATE);

	ReadCipher(CIPHER_FILE, CIPHER_SIZE, KEY_SIZE);
	ReadNGraphs(LOGWEIGHT);
	InitializeDictionary();
	
	Intro();

	do
	{
		PrintMenu();

		cin>>selection;
		while(selection < 1 || selection > 6)
			cin>>selection;

		switch(selection)
		{
			case 1: //RUN ONE SINGLE SIMULATION

				Simulate(NUM_KEYS, CIPHER_FILE, CIPHER_SIZE, KEY_SIZE, GENERATIONS, CIPHER_HEIGHT, CIPHER_WIDTH, SURVIVAL_RATE, BREED_RATE, MUTATE_RATE, BI_BONUS, TRI_BONUS, TETRA_BONUS, PENTA_BONUS);
				
				cout<<"SIMULATION COMPLETE."<<endl;

			break;

			case 2: //LOOP SIMULATIONS UNTIL USER EXITS

				Simulations(NUM_KEYS, CIPHER_FILE, CIPHER_SIZE, KEY_SIZE, GENERATIONS, CIPHER_HEIGHT, CIPHER_WIDTH, SURVIVAL_RATE, BREED_RATE, MUTATE_RATE, BI_BONUS, TRI_BONUS, TETRA_BONUS, PENTA_BONUS);
				
				cout<<endl<<"SIMULATIONS COMPLETE."<<endl;

			break;

			case 3: //SEED KEYS


			break;

			case 4: //RUN CONFIGURATOR

				Configure(NUM_KEYS, CIPHER_FILE, CIPHER_SIZE, KEY_SIZE, GENERATIONS, LOGWEIGHT, CIPHER_HEIGHT, CIPHER_WIDTH, SURVIVAL_RATE, BREED_RATE, MUTATE_RATE, BI_BONUS, TRI_BONUS, TETRA_BONUS, PENTA_BONUS);

			break;
		}

	}while(selection != 6);
}

/////
// Defaults() TO SET THE SIMULATION PARAMETERS TO DEFAULT VALUES
/////

void GA::Defaults(int &CIPHER_SIZE, int &CIPHER_WIDTH, int &CIPHER_HEIGHT, int &KEY_SIZE, int &NUM_KEYS, int &GENERATIONS, bool &LOGWEIGHT, string &CIPHER_FILE, double &BI_BONUS, double &TRI_BONUS, double &TETRA_BONUS, double &PENTA_BONUS, double &SURVIVAL_RATE, double &BREED_RATE, double &MUTATE_RATE)
{
	/////
	// DEFAULT VALUES FOR SIMULATION PARAMETERS - SET TO 340 CIPHER BY DEFAULT
	/////
	CIPHER_SIZE = 340;
	CIPHER_WIDTH = 17;
	CIPHER_HEIGHT = 20;
	KEY_SIZE = 63;
	NUM_KEYS = 1000;
	GENERATIONS = 100;

	LOGWEIGHT = false;

	CIPHER_FILE = "zodiac340.txt";

	BI_BONUS = 1;
	TRI_BONUS = 1;
	TETRA_BONUS = 1;
	PENTA_BONUS = 1;

	SURVIVAL_RATE = 0.1;
	BREED_RATE = 0.4;
	MUTATE_RATE = 1.0 - BREED_RATE;
}

/////
// Simulate() TO RUN ONE ENTIRE GENETIC SIMULATION
/////

void GA::Simulate(int NUM_KEYS, string CIPHER_FILE, int CIPHER_SIZE, int KEY_SIZE, int GENERATIONS, int CIPHER_HEIGHT, int CIPHER_WIDTH, double SURVIVAL_RATE, double BREED_RATE, double MUTATE_RATE, double BI_BONUS, double TRI_BONUS, double TETRA_BONUS, double PENTA_BONUS)
{
	keyType bestKey;

	bestKey.score = 0;

	ReadCipher(CIPHER_FILE, CIPHER_SIZE, KEY_SIZE);
	InitializeKeys(NUM_KEYS, KEY_SIZE);

	for(int g = 0; g < GENERATIONS; g++)
	{
		SolveKeys(NUM_KEYS, KEY_SIZE, CIPHER_SIZE);
		ScoreKeys(NUM_KEYS, CIPHER_SIZE, KEY_SIZE, bestKey, BI_BONUS, TRI_BONUS, TETRA_BONUS, PENTA_BONUS);

		if(rePrint)
		{
			PrintCipher(bestKey, CIPHER_HEIGHT, CIPHER_WIDTH, KEY_SIZE);
			g = 0;

			rePrint = false;
		}
		
		SortKeys(NUM_KEYS);
		MateAndMutate(NUM_KEYS, KEY_SIZE, SURVIVAL_RATE, BREED_RATE, MUTATE_RATE);
	}
}

/////
// Simulations() TO RUN MULTIPLE GENETIC SIMULATIONS
/////

void GA::Simulations(int NUM_KEYS, string CIPHER_FILE, int CIPHER_SIZE, int KEY_SIZE, int GENERATIONS, int CIPHER_HEIGHT, int CIPHER_WIDTH, double SURVIVAL_RATE, double BREED_RATE, double MUTATE_RATE, double BI_BONUS, double TRI_BONUS, double TETRA_BONUS, double PENTA_BONUS)
{
	int SIMULATIONS;

	keyType bestKey;

	bestKey.score = 0;

	ReadCipher(CIPHER_FILE, CIPHER_SIZE, KEY_SIZE);
	
	cout<<endl<<"ENTER NUMBER OF SIMULATIONS: "<<endl;
	cin>>SIMULATIONS;

	for(int s = 1; s <= SIMULATIONS; s++)
	{
		InitializeKeys(NUM_KEYS, KEY_SIZE);

		for(int g = 0; g < GENERATIONS; g++)
		{
			SolveKeys(NUM_KEYS, KEY_SIZE, CIPHER_SIZE);
			ScoreKeys(NUM_KEYS, CIPHER_SIZE, KEY_SIZE, bestKey, BI_BONUS, TRI_BONUS, TETRA_BONUS, PENTA_BONUS);
		
			if(rePrint)
			{
				PrintCipher(bestKey, CIPHER_HEIGHT, CIPHER_WIDTH, KEY_SIZE);
				g = 0;

				rePrint = false;
			}
		
			SortKeys(NUM_KEYS);
			MateAndMutate(NUM_KEYS, KEY_SIZE, SURVIVAL_RATE, BREED_RATE, MUTATE_RATE);
		}

		PrintCipher(bestKey, CIPHER_HEIGHT, CIPHER_WIDTH, KEY_SIZE);
		cout<<(int)(((double)s / (double)SIMULATIONS) * 100)<<"% COMPLETE."<<endl;
	}
}

/////
// readCipher() FUNCTION TO READ CIPHER TEXT INTO char cipher[] ARRAY
// ALSO FILLS UNIQUE KEY STRING WITH UNIQUE SYMBOLS
/////

void GA::ReadCipher(string CIPHER_FILE, int CIPHER_SIZE, int KEY_SIZE)
{
	int index = 0;

	bool insert;

	ifstream inFile;
	
	inFile.open(CIPHER_FILE.c_str());

	if(!inFile)
	{
		cout<<endl<<"Could not open cipher file."<<endl;
		cout<<"Press any key to exit..."<<endl;
		cin.get();
		exit(1);
	}

	inFile>>CIPHER;

	for(int i = 0; i < CIPHER_SIZE; i++)
	{
		insert = true;

		for (int c = 0; c < KEY_SIZE; c++)
        {
			if (CIPHER[i] == UNIQUES[c])
				insert = false;
        }

		if(insert)
		{
			UNIQUES[index] = CIPHER[i];
			index++;
		}
	}

	ifstream letterFile ("letterFreqs.txt");
	if(!letterFile)
	{
		cout<<endl<<"Could not open letterFreqs.txt file."<<endl;
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

/////
// ReadNGraphs() TO READ IN NGRAPH DATA FROM ANALYZED TEXT FILES 
/////

void GA::ReadNGraphs(bool LOGWEIGHT)
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

/////
// InitializeKeys() TO RANDOMLY GENERATE THE INITIAL KEY POPULATION OF SIZE NUM_KEYS
/////

void GA::InitializeKeys(int NUM_KEYS, int KEY_SIZE)
{
	time_t seconds;
	time(&seconds);

	srand((unsigned int) seconds);

	for(int k = 0; k < NUM_KEYS; k++)
	{
		keys[k].score = 0;

		if(seedKeys)
		{
			for(int c = 0; c < KEY_SIZE; c++)
				keys[k].key[c] = seed[c];

			MateAndMutate(NUM_KEYS, KEY_SIZE, .01, 0.0, 1.0);
		}
		else
		for(int c = 0; c < KEY_SIZE; c++)
			keys[k].key[c] = (char)(65 + (rand() % 26));
	}
}

/////
// InitializeDictionary() TO RANDOMLY GENERATE THE INITIAL KEY POPULATION OF SIZE NUM_KEYS
/////

void GA::InitializeDictionary()
{
	for (int i = 0; i < 101; i++) 
	{								
	  Scores[i] = i;
	}

	Dictionary.FillDictionary(Dictionary, DICTIONARY_FILE, Scores, MAX_WORD_SCORE); 

	Dictionary.SetCustomWordScore(Dictionary, "enter word here", 99, Scores);
}

/////
// SolveKeys() FUNCTION TO SOLVE EACH OF THE KEYS
/////

void GA::SolveKeys(int NUM_KEYS, int KEY_SIZE, int CIPHER_SIZE)
{
	for(int i = 0; i < NUM_KEYS; i++)
		for(int u = 0; u < KEY_SIZE; u++)
			for(int c = 0; c < CIPHER_SIZE; c++)
				if(CIPHER[c] == UNIQUES[u])
					keys[i].solution[c] = keys[i].key[u];
}

/////
// ScoreKeys() FUNCTION TO SCORE EACH OF THE KEYS
/////

void GA::ScoreKeys(int NUM_KEYS, int CIPHER_SIZE, int KEY_SIZE, keyType & bestKey, double BI_BONUS, double TRI_BONUS, double TETRA_BONUS, double PENTA_BONUS)
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

/////
// SortKeys() TO SORT THE KEYS BASED ON KEY SCORE
/////

void GA::SortKeys(int NUM_KEYS)
{
	keyType temp;
    
	int lowestLoc;
	float lowestScore;
	int countdown = NUM_KEYS - 1;

	for(int a = 0; a < NUM_KEYS; a++)
	{
		lowestScore = 100;

		for(int u = 0; u < countdown; u++)
			if(keys[u].score < lowestScore)
			{
				lowestLoc = u;
				lowestScore = keys[u].score;
			}

		temp = keys[countdown];
		keys[countdown] = keys[lowestLoc];
		keys[lowestLoc] = temp;

		countdown--;
	}
}

/////
// MateAndMutate() FUNCTION TO GENERATE NEW POPULATION
/////

void GA::MateAndMutate(int NUM_KEYS, int KEY_SIZE, double SURVIVAL_RATE, double BREED_RATE, double MUTATE_RATE)
{
	int donor;
	int mother; 
	int father; 
	int mutant;
	int mutation;
	int numMutations;
	int mutationLocation;
	   
	int survivingPopulation = (int)((double)NUM_KEYS * SURVIVAL_RATE);
	int numChildren = (int)((double)(NUM_KEYS - survivingPopulation) * BREED_RATE);
	int numMutants = (int)((double)(NUM_KEYS - survivingPopulation) * MUTATE_RATE);

	string mutantString;

	keyType newKey;

	time_t seconds;
	time(&seconds);

	srand((unsigned int) seconds);

    for(int i = survivingPopulation; i < survivingPopulation + numChildren - 1; i++)
	{                
		mother = (rand() % survivingPopulation);
		father = (rand() % survivingPopulation);

		for (int c = 0; c < KEY_SIZE; c++) 
		{
			donor = rand() % 2;
                
			if (donor == 0) 
				newKey.key[c] = keys[mother].key[c];
			else 
				newKey.key[c] = keys[father].key[c];
		}

		newKey.score = 0;

		keys[i] = newKey;
	}

	for(int i = NUM_KEYS - numMutants - 1; i < NUM_KEYS; i++)
	{
		mutant = rand() % survivingPopulation;
		mutantString = keys[mutant].key;

		numMutations = (rand() % KEY_SIZE) + 1;

		for(int m = 0; m < numMutations; m++)
		{
			mutation = (rand() % 26) + 65;

			mutationLocation = rand() % KEY_SIZE;

			mutantString[mutationLocation] = mutation;
		}

		for(int c = 0; c < KEY_SIZE; c++)
			keys[i].key[c] = mutantString[c];

		keys[i].score = 0;
	}
}

/////
// PrintCipher() TO PRINT THE CIPHER AND CURRENT BEST SOLUTION
/////

void GA::PrintCipher(keyType key, int CIPHER_HEIGHT, int CIPHER_WIDTH, int KEY_SIZE)
{
	int s = 0;
	int l = 0;

	cout<<endl;
	cout<<"------------------CURRENT BEST SOLUTION-------------------"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
	cout<<endl;
	cout<<"              CIPHER                 SOLUTION            "<<endl;
	cout<<endl;

	for(int h = 0; h < CIPHER_HEIGHT; h++)
	{
		cout<<"         ";

		for(int i = 0; i < CIPHER_WIDTH; i++)
		cout<<CIPHER[s++];

		cout<<"   =   ";

		for(int j = 0; j < CIPHER_WIDTH; j++)
		cout<<key.solution[l++];

		cout<<endl;
	}

	cout<<endl;
	cout<<"                  BEST SCORE = "<<key.score<<endl;
	//cout<<"               AVERAGE SCORE = "<<averageScore<<endl;

	cout<<endl<<"UNIQUES  = ";
	for(int i = 0; i < KEY_SIZE; i++)
		cout<<UNIQUES[i];
	cout<<endl<<"BEST KEY = ";
	for(int s = 0; s < KEY_SIZE; s++)
		cout<<key.key[s];
	cout<<endl<<endl;
}

/////
// Intro() FUNCTION TO OUTPUT THE FRIKIN' SWEET zGAD ASCII TEXT LOGO
/////

void GA::Intro()
{
	cout<<"                            @-                             "<<endl;
	cout<<"                        .:--@#--.                          "<<endl;
	cout<<"                    =@@@@@%*@@#@@@@@*:                     "<<endl;
	cout<<"                 =@@@*:     @-    .=%@@#                   "<<endl;
	cout<<"               *@@=         @+        :@@@                 "<<endl;
	cout<<"             :@@:           @+           %@*               "<<endl;
	cout<<"            *@+             @+            .@@              "<<endl;
	cout<<"           #@:              @+              @@             "<<endl;
	cout<<"          +@.               @+               @@            "<<endl;
	cout<<"          @+    +%@@@@@*:   @+  :%##%+      #%@%#%%#*:     "<<endl;
	cout<<"         #@   %@@@@@@@@@@@  @=  @@@@@@      @@@@@@@@@@@.   "<<endl;
	cout<<"  #**++==@@  @@@@+    ##**  @- *@@%%@@@     @@@@.  *@@@@   "<<endl;
	cout<<" .@@@@@@@@@=+@@@@---: .   :=@*:@@@*+@@@+=++=@@@@*:- %@@@+  "<<endl;
	cout<<"     =@@@@=+%@@@@=+#@@@@@@@=@#@@@%..#@@@#***@@@@%+* +@@@*  "<<endl;
	cout<<"    %@@%:@   @@@@   :.-@@@- @%@@@@++@@@@*   @@@@    @@@@=  "<<endl;
	cout<<"  %@@@#  @*  *@@@@=. .*@@@+ @@@@@@@@@@@@@:  @@@@*=+@@@@@   "<<endl;
	cout<<" #@@@@@@@@@-  =@@@@@@@@@@#  @@@@=    :@@@@  @@@@@@@@@@#    "<<endl;
	cout<<" .=-------@*    .-+**+-.    @#:-      ---=. :=@+---:.      "<<endl;
	cout<<"          -@:               @-               @@            "<<endl;
	cout<<"           *@-              @+              @@             "<<endl;
	cout<<"            =@#             @+            -@@              "<<endl;
	cout<<"             .@@-           @+          .@@=               "<<endl;
	cout<<"               =@@*         @+        -@@#                 "<<endl;
	cout<<"                 -@@@#-     @-    :+@@@*                   "<<endl;
	cout<<"                    -#@@@@@#@@%@@@@@+                      "<<endl;
	cout<<"                         .::@#..                           "<<endl;
	cout<<"                            @-                             "<<endl;
	cout<<endl;
	cout<<"-------------ZODICAC GENETIC ALGORITHM DECODER------------"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
	cout<<endl;
	cout<<" zGAD, THE ZODIAC GENETIC ALGORITHM DECODER IS A DISTRIB- "<<endl;
	cout<<" UTED PROGRAM THAT USES A GENETIC ALGORITHM IN AN ATTEMPT "<<endl;
	cout<<" TO FIND A SOLUTION TO THE UNSOLVED ZODIAC 340 CIPHER.    "<<endl;
	cout<<endl;
	cout<<"PRESS ENTER TO CONTINUE..."<<endl;
	cin.get();
	cout<<endl;
}

/////
// PrintMenu() FUNCTION TO PRINT THE MAIN zGAD MENU
/////

void GA::PrintMenu()
{
	cout<<endl;
	cout<<"----------------------zGAD MAIN MENU----------------------"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
	cout<<endl;
	cout<<" 1. RUN SIMULATION"<<endl;
	cout<<" 2. LOOP SIMULATION"<<endl;
	cout<<" 3. SEED KEYS"<<endl;
	cout<<" 4. CONFIGURE SETTINGS"<<endl;
	cout<<" 5. TEST SCORING ALGORITHM"<<endl;
	cout<<" 6. EXIT zGAD"<<endl;
	cout<<endl;
	cout<<"PLEASE ENTER A SELECTION..."<<endl;
}

/////
// Configure() FUNCTION TO CONFIGURE THE PROGRAM'S SIMULATION PARAMETERS
/////

void GA::Configure(int &NUM_KEYS, string &CIPHER_FILE, int &CIPHER_SIZE, int &KEY_SIZE, int &GENERATIONS, bool &LOGWEIGHT, int &CIPHER_HEIGHT, int &CIPHER_WIDTH, double &SURVIVAL_RATE, double &BREED_RATE, double &MUTATE_RATE, double &BI_BONUS, double &TRI_BONUS, double &TETRA_BONUS, double &PENTA_BONUS)
{


	int restore = -1;

	char differentCipher;

	ifstream testFile;

	cout<<endl;
	cout<<"----------------------zGAD CONFIGURATOR-------------------"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
	cout<<endl;
	
	cout<<" 1. CONFIGURE NEW SETTINGS"<<endl;
	cout<<" 2. RESTORE DEFAULTS"<<endl;
	
	cout<<endl<<"PLEASE ENTER A SELECTION:"<<endl;
	while(restore < 1 || restore > 2)
		cin>>restore;

	if(restore == 2)
	{
		cout<<endl<<"RESTORING DEFAULTS..."<<endl;
		
		Defaults(CIPHER_SIZE, CIPHER_WIDTH, CIPHER_HEIGHT, KEY_SIZE, NUM_KEYS, GENERATIONS, LOGWEIGHT, CIPHER_FILE, BI_BONUS, TRI_BONUS, TETRA_BONUS, PENTA_BONUS, SURVIVAL_RATE, BREED_RATE, MUTATE_RATE);
		
		cout<<endl<<"DONE."<<endl;
	}
	else
	{	
		cout<<endl<<"WOULD YOU LIKE TO USE A DIFFERENT CIPHER? (Y/N)"<<endl;
		cin>>differentCipher;

		while(differentCipher != 'Y' && differentCipher != 'N' && differentCipher != 'y' && differentCipher != 'n')
		{
			cout<<"INVALID ENTRY. PLEASE RE-ENTER:"<<endl;
			cin>>differentCipher;
		}

		
		if(differentCipher == 'y')
			differentCipher -= 32;

		if(differentCipher == 'Y')
		{
			cout<<endl<<"ENTER THE NAME/PATH OF THE NEW CIPHER: "<<endl;
			cin>>CIPHER_FILE;

			testFile.open(CIPHER_FILE.c_str());

			while(!testFile)
			{
				cout<<endl<<"INVALID FILE. RETURNING TO MAIN MENU..."<<endl;
				
				return;
			}
		
			cout<<endl<<"ENTER THE SIZE OF THE CIPHER:"<<endl;
			cin>>CIPHER_SIZE;

			while(CIPHER_SIZE < 1 || CIPHER_SIZE > _MAX_CIPHER)
			{
				cout<<endl<<"INVALID SIZE. PLEASE RE-ENTER:"<<endl;
				cin>>CIPHER_SIZE;
			}

			cout<<endl<<"ENTER THE WIDTH OF THE CIPHER FOR DISPLAY: (IDEAL = 17)"<<endl;
			cin>>CIPHER_WIDTH;

			while(CIPHER_WIDTH < 1 || CIPHER_WIDTH > 30)
			{
				cout<<endl<<"INVALID WIDTH. PLEASE RE-ENTER:"<<endl;
				cin>>CIPHER_WIDTH;
			}

			if(CIPHER_SIZE % CIPHER_WIDTH == 0)
				CIPHER_HEIGHT = CIPHER_SIZE / CIPHER_WIDTH;
			else
				CIPHER_HEIGHT = (CIPHER_SIZE / CIPHER_WIDTH) + 1;
		}//END IF DIFF CIPHER

		cout<<endl<<"ENTER THE NUMBER OF KEYS PER GENERATION: (10 - "<<_MAX_KEYS<<")"<<endl;		
		cin>>NUM_KEYS;

		while(NUM_KEYS < 10 || NUM_KEYS > _MAX_KEYS)
		{
			cout<<endl<<"INVALID NUMBER. PLEASE RE-ENTER:"<<endl;
			cin>>NUM_KEYS;
		}

		cout<<endl<<"ENTER THE NUMBER OF GENERATIONS BEFORE TERMINATION: (1 - "<<_MAX_GENERATIONS<<")"<<endl;		
		cin>>GENERATIONS;

		while(GENERATIONS < 1 || GENERATIONS > _MAX_GENERATIONS)
		{
			cout<<endl<<"INVALID NUMBER. PLEASE RE-ENTER:"<<endl;
			cin>>GENERATIONS;
		}

		cout<<endl<<"ENTER THE SURVIVAL RATE PER GENERATION: (0.0 - 1.0)"<<endl;		
		cin>>SURVIVAL_RATE;

		while(SURVIVAL_RATE < 0.0 || SURVIVAL_RATE > 1.0)
		{
			cout<<endl<<"INVALID RATE. PLEASE RE-ENTER:"<<endl;
			cin>>SURVIVAL_RATE;
		}

		cout<<endl<<"ENTER THE BREED RATE PER GENERATION: (0.0 - 1.0)"<<endl;
		cout<<"NOTE THAT THE BREEDING RATE WILL BE 1.0 - BREED RATE."<<endl;
		cin>>BREED_RATE;

		while(BREED_RATE < 0.0 || BREED_RATE > 1.0)
		{
			cout<<endl<<"INVALID RATE. PLEASE RE-ENTER:"<<endl;
			cin>>BREED_RATE;
		}

		MUTATE_RATE = 1.0 - BREED_RATE;

		cout<<endl<<"ENTER THE SCORING BONUS FOR BIGRAPHS: (-"<<_MAX_BONUS<<" - "<<_MAX_BONUS<<")"<<endl;		
		cin>>BI_BONUS;

		while(BI_BONUS < (-1 * _MAX_BONUS) || BI_BONUS > _MAX_BONUS)
		{
			cout<<endl<<"INVALID BONUS. PLEASE RE-ENTER:"<<endl;
			cin>>BI_BONUS;
		}

		cout<<endl<<"ENTER THE SCORING BONUS FOR TRIGRAPHS: (-"<<_MAX_BONUS<<" - "<<_MAX_BONUS<<")"<<endl;		
		cin>>TRI_BONUS;

		while(TRI_BONUS < (-1 * _MAX_BONUS) || TRI_BONUS > _MAX_BONUS)
		{
			cout<<endl<<"INVALID BONUS. PLEASE RE-ENTER:"<<endl;
			cin>>TRI_BONUS;
		}

		cout<<endl<<"ENTER THE SCORING BONUS FOR TETRAGRAPHS: (-"<<_MAX_BONUS<<" - "<<_MAX_BONUS<<")"<<endl;		
		cin>>BI_BONUS;

		while(TETRA_BONUS < (-1 * _MAX_BONUS) || TETRA_BONUS > _MAX_BONUS)
		{
			cout<<endl<<"INVALID BONUS. PLEASE RE-ENTER:"<<endl;
			cin>>TETRA_BONUS;
		}

		cout<<endl<<"ENTER THE SCORING BONUS FOR PENTAGRAPHS: (-"<<_MAX_BONUS<<" - "<<_MAX_BONUS<<")"<<endl;		
		cin>>PENTA_BONUS;

		while(PENTA_BONUS < (-1 * _MAX_BONUS) || PENTA_BONUS > _MAX_BONUS)
		{
			cout<<endl<<"INVALID BONUS. PLEASE RE-ENTER:"<<endl;
			cin>>PENTA_BONUS;
		}

	}//END ELSE
}

/////
// END of zGAD.cpp
/////