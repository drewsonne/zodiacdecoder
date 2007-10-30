// zLauncher.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <math.h>

using namespace zLauncher;
using namespace std;

stringstream KeyStream (stringstream::in | stringstream::out);
void zRnd();

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}

void zRnd()
{
	int SIMULATIONS=10000;
	// to this from stream
	string key;
	// iterate for n simulations
	for(int s = 1; s <= SIMULATIONS; s++)
	{
		// send random key to stream
		for(int j = 0; j < 63; j++){
			KeyStream<<(char)(65 + (rand() % 26));
		}
		// test reading from keystream
		KeyStream>>key;
		// comment out for release
		cout<<key<<" "<<(int)(((double)s / (double)SIMULATIONS) * 100)<<"%"<<endl;
		KeyStream.clear();
	}
}
