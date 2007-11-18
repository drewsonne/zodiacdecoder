/*
	zWordReader.cpp
	version 1.0.0

	Tanner Oakes
	11.18.07
	
	CSCE 3210 Symbolic Processing

	This will scan a hard coded file called "zodiac.txt" and count the
	occurences of each word. It will create a temporary file called "filter.txt"
	and a final file called "results.txt" . It is unsorted and I just use Excel to
	sort the data. Further down, if you un-comment some code you can switch from
	space-delimited to tab-delimited data, if you prefer.
	
*/

#include <iostream>
#include <fstream>

using namespace std;

bool filterFile();
bool scanFile();
bool hasBeenScanned(string test);

int main()
{
	filterFile();
	scanFile();
	
	return 0;
}

// Test to see if the current word has already been counted
bool hasBeenScanned(string test)
{
	ifstream file;
	file.open( "results.txt" );
	
	string scan;
	
	if ( !file.is_open() )
	{
		cout << "hasBeenScanned - ERROR OPENING FILE" << endl;
		return false;
	}
	
	do
	{
		file >> scan;
		if ( scan == test )
			return true;
	} while ( !file.eof() );
	
	return false;
}

// Count all of the occurences of unique words in the file "zodiac.txt"
bool scanFile()
{
	ifstream file;
	ifstream scanner;
	ofstream outfile;
	file.open( "filter.txt" );
	outfile.open( "results.txt" );
	scanner.open( "filter.txt" );
	
	string current, scan;
	int count;
	
	if ( !file.is_open() )
	{
		cout << "ERROR OPENING FILE" << endl;
		return false;
	}
	
	do
	{
		file >> current;
		count = 0;
		if ( !hasBeenScanned( current ) )
		{
			do
			{
				scanner >> scan;
				if ( scan == current )
					count++;
			} while ( !scanner.eof() );
			scanner.close();
			scanner.open( "filter.txt" );
			outfile << current << " ";
			
//>>>>>>>>>	IF YOU WANT TAB-DELIMITED DATA REMOVE THESE COMMENTS

			/*
			
			cout << "\t";
			if ( current.length() < 8 )
				outfile << "\t";
				
			*/
			
			outfile << count << endl;
		}
	} while ( !file.eof() );
}

// Remove any non-alphabetic characters from the file
bool filterFile()
{
	ifstream file;
	ofstream outfile;
	file.open( "zodiac.txt" );
	outfile.open( "filter.txt" );
	char work;
	
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
			if ( isalpha(work) )
			{
    			if ( isupper(work) )
                    work = tolower(work);
                outfile << work;    
            }
            else if ( work == ' ' )
            	outfile << work;
            else if ( work == '\n' )
            	outfile << " ";

		} while ( !file.eof() );
	}
	
	outfile.close();
	file.close();
}