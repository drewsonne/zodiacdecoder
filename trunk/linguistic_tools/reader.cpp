/*
	Tanner Oakes
	10.15.07
	
	CSCE 3210 Symbolic Processing
	
	IMPORTANT NOTE: Currently this program uses matrices to organize its data.
	In order to accomodate up to n==5 n-graphs, I will need to rethink our data structures.
	So, right now this program will do n==4 n-graphs, with the n==5 sections commented out.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

const char* OUTPUT = "datafile.txt";
const char* REPORT = "report.txt";

bool processInput(string input);
void initDatafile(ofstream& data);
void readDatafile(int* freq, int bigram[26*26], int trigram[26*26][26], int fourgram[26*26][26*26] );
void writeDatafile( int* freq, int bigram[26*26], int trigram[26*26][26], int fourgram[26*26][26*26] );
void printReport();
char convertInt(int a);

int main()
{
	string input;
	ofstream data;
	
	data.open( OUTPUT );
	initDatafile( data );
	data.close();
	
	do
	{
    	// Accept user input
	   cout << "File to analyze (Type report to print report and exit): " << endl;
	   cin >> input;
	   if ( input == "report" )
	   {
	       printReport();
	       return 0;
	   }
	   // Process input file
	   processInput(input);
	} while ( 1==1 );
	
	return 0;
}

char convertInt(int a)
{
    return (char)(a + 97);
}

void printReport()
{
    int freq[26];
    int bigram[26*26];
    int trigram[26*26][26];
    int fourgram[26*26][26*26];
    ofstream report;
    ofstream output;
    int totalFreq = 0;
    int totalBi = 0;
    int totalTri = 0;
    int totalFour = 0;
    int totalFive = 0;
    
    readDatafile( freq, bigram, trigram, fourgram );
    
    // Count character frequency data
    for (int i = 0; i < 26; i++)
        totalFreq += freq[i];
    // Count bigram data
    for (int i = 0; i < 26*26; i++)
        totalBi += bigram[i];
    // Count trigram data
    for (int i = 0; i < 26*26; i++)
        for (int b = 0; b < 26; b++)
        	totalTri += trigram[i][b];
    // Count four-gram data
    for (int i = 0; i < 26*26; i++)
        for (int a = 0; a < 26*26; a++)
            totalFour += fourgram[i][a];
    report.open( REPORT );
    

    // Report character frequency
    output.open( "char_freqs.txt" );
	for (int i = 0; i < 26; i++)
	{
		cout << convertInt(i) << " = " << freq[i] << " hits. " << (float)((float)freq[i] / (float)totalFreq) * 100 << "%" << endl;
		output << (char)toupper( convertInt(i) ) << " " << freq[i] << " " << (float)((float)freq[i] / (float)totalFreq) << endl;
    }
    output.close();

	
	// Scan bigram chart and report any hits > 0
	output.open( "bigraphs.txt" );
//	cout << endl << endl << "DIGRAM ANALYSIS:" << endl;
	report << endl << endl << "DIGRAM ANALYSIS:" << endl;
	for (int i = 0; i < 26*26; i++)
		if (bigram[i] > 0)
		{
			//cout << convertInt(i) << " " << convertInt(a) << " = " << bigram[i][a] << " hits. " << (float)((float)bigram[i][a] / (float)totalBi) * 100 << "%" << endl;
			//report << convertInt(i) << " " << convertInt(a) << " = " << bigram[i][a] << " hits. " << (float)((float)bigram[i][a] / (float)totalBi) * 100 << "%" << endl;
           	output << (char)toupper( convertInt(i / 26) ) << (char)toupper( convertInt(i % 26) ) << " " << bigram[i] << " " << (float)((float)bigram[i] / (float)totalBi) << endl;
        }
    output.close();
    
	// Scan trigram chart and report any hits > 0
	output.open( "trigraphs.txt" );
//	cout << endl << endl << "TRIGRAM ANALYSIS:" << endl;
	report << endl << endl << "TRIGRAM ANALYSIS:" << endl;
	for (int i = 0; i < 26*26; i++)
		for (int b = 0; b < 26; b++)
			if (trigram[i][b] > 0)
			{
				//cout << convertInt(i) << " " << convertInt(a) << " " << convertInt(b) << " = " << trigram[i][a][b] << " hits. " << (float)((float)trigram[i][a][b] / (float)totalTri) * 100 << "%" << endl;
				//report << convertInt(i) << " " << convertInt(a) << " " << convertInt(b) << " = " << trigram[i][a][b] << " hits. " << (float)((float)trigram[i][a][b] / (float)totalTri) * 100 << "%" << endl;
               	output << (char)toupper( convertInt(i / 26) ) << (char)toupper( convertInt( i % 26 ) ) << (char)toupper( convertInt(b) ) << " " << trigram[i][b] << " " << (float)trigram[i][b] / (float)totalTri << endl;
            }
   output.close();

	// Scan four-gram chart and report any hits > 0
	output.open( "tetragraphs.txt" );
	for (int i = 0; i < 26*26; i++)
		for (int a = 0; a < 26*26; a++)
		{
			if (fourgram[i][a] >= 1)
			{
				output << (char)toupper( convertInt(i / 26) ) << (char)toupper( convertInt( i % 26 ) ) << (char)toupper( convertInt( a / 26 ) ) << (char)toupper( convertInt( a % 26 ) ) << " " << fourgram[i][a] << " " << (float)fourgram[i][a] / (float)totalFour << endl;
			}
			if (fourgram[i][a] >= 500)
				cout << i << " " << a << endl;
		}
    output.close();
}

void initDatafile(ofstream& data)
{
	cout << "INITIALIZING DATA" << endl;
    // Initialize character frequency data
	cout << "initializing character frequency..." << endl;
    for (int i = 0; i < 26; i++)
        data << "0 ";
    // Init bigram data
	cout << "initializing bigraph data..." << endl;
    for (int i = 0; i < 26 * 26; i++)
            data << "0 ";

    // Init trigram data
	cout << "initializing trigraph data..." << endl;
    for (int i = 0; i < 26 * 26; i++)
	     for (int b = 0; b < 26; b++)
           data << "0 ";

    // Init 4-gram data
	cout << "initializing 4-graph data..." << endl;
    for (int i = 0; i < 26 * 26; i++)
    	for (int a = 0; a < 26 * 26; a++)
 				data << "0 ";

	cout << endl << endl;
}

void readDatafile(int* freq, int bigram[26*26], int trigram[26*26][26], int fourgram[26*26][26*26])
{
	ifstream data;
    data.open( OUTPUT );
    for (int i = 0; i < 26; i++)
        data >> freq[i];
    for (int i = 0; i < 26 * 26; i++)
        data >> bigram[i];
    for (int i = 0; i < 26 * 26; i++)
        for (int b = 0; b < 26; b++)
            data >> trigram[i][b];
    for (int i = 0; i < 26*26; i++)
    	for (int a = 0; a < 26*26; a++)
    				data >> fourgram[i][a];
    data.close();
}

void writeDatafile(int* freq, int bigram[26*26], int trigram[26*26][26], int fourgram[26*26][26*26] )
{
    ofstream data;
    data.open( OUTPUT );
    for (int i = 0; i < 26; i++)
        data << freq[i] << " ";
    for (int i = 0; i < 26*26; i++)
        data << bigram[i] << " ";
    // Write trigram data
    for (int i = 0; i < 26 * 26; i++)
        for (int b = 0; b < 26; b++)
            data << trigram[i][b] << " ";
    // Write fourgram data
    for (int i = 0; i < 26*26; i++)
        for (int a = 0; a < 26*26; a++)
            data << fourgram[i][a] << " ";
	data.close();
}

bool processInput(string input)
{
    ifstream file;
    ofstream outfile;
    char work;
    int freq[26];
    int bigram[26*26];
    int trigram[26*26][26];
    int fourgram[26*26][26*26];
    int first;
    int second;
    int third;
    int fourth;
    readDatafile( freq, bigram, trigram, fourgram );

    file.open( input.c_str() );
    outfile.open( "temp.txt" );
	if ( !file.is_open() )
	{
		cout << "ERROR OPENING FILE" << endl;
		return false;	
    }
	else
	{
		do
		{
			work = file.get();
			if ( work == '&' )
            {
                outfile << (int)'a' - 97 << " " << (int)'n' - 97 << " " << (int)'d' - 97 << " ";
            }
			if ( isalpha(work) )
			{
    			if ( isupper(work) )
                    work = tolower(work);
                freq[ (int)work - 97 ] += 1;
                outfile << (int)work - 97 << " ";    
            }

		} while ( !file.eof() );
	}
	outfile.close();
	file.close();

	// Scan bigrams
	cout << "scanning bigraphs..." << endl;
	file.open( "temp.txt" );
	if ( !file.is_open() )
	{
	   cout << "ERROR OPENING FILE" << endl;
	   return false;
	}
	else
	{
	   file >> first;
	   do
	   {
	       file >> second;
	       bigram[first*26 + second] += 1;
	       first = second;
	   } while ( !file.eof() );
	}
	file.close();

	// Scan trigrams
	cout << "scanning trigraphs..." << endl;
	file.open( "temp.txt" );
	if ( !file.is_open() )
	{
        cout << "ERROR OPENING FILE" << endl;
        return false;
	}
	else
	{
        file >> first;
        file >> second;
        do
        {
            file >> third;
            trigram[first*26 + second][third] += 1;
            first = second;
            second = third;
        } while ( !file.eof() );
	}
	file.close();
	
	// Scan fourgram
	cout << "scanning fourgraphs.." << endl;
	file.open( "temp.txt" );
	if ( !file.is_open() )
	{
		cout << "ERROR OPENING FILE" << endl;
		return false;
	}
	else
	{
		file >> first;
		file >> second;
		file >> third;
		do
		{
			file >> fourth;
			fourgram[(first*26 + second)][(third*26 + fourth)] += 1;
			first = second;
			second = third;
			third = fourth;
		} while ( !file.eof() );
	}
	file.close();

	// Write data to file
	cout << "writing data to file..." << endl;
	writeDatafile( freq, bigram, trigram, fourgram );
	cout << "done." << endl << endl;
	return true;
}