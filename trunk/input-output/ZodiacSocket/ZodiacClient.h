#pragma once

#include<winsock2.h>
#include <winsock2.h>
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>
#include <iostream.h>
#include <fstream.h>
#include <cstdlib>




class ZodiacClient
{
public:
	ZodiacClient(void);
	~ZodiacClient(void);
	bool initializeClient();
	bool setServer(char* server);
	bool Connect(int ip_b1, int ip_b2, int ip_b3, int ip_b4, int port);
	bool sendKey(char* key, int key_size);
	void slayClient();

	
	/*

	------------
	Commented out until SECOND PHASE (program intergration)
	------------
	static int getNumberOfKeys(){
		int counter = 0;
		char* key;

		ifstream inClientFile("keys.dat", ios::in);

		if(!inClientFile)
			return 0;
		else
			while(inClientFile >> key){
				counter++;
			}

		return counter;
	}

	static void appendKey(char* key){

		ofstream outClientFile("keys.dat", ios::ate);

		if(!outClientFile){
			cerr<< "File could not be opened\n";
			exit(1);
		}

		outClientFile << key;
	}

	static const char* getKey(){
		
		char
		ifstream inClientFile("keys.dat", ios::in);

		if(!inClientFile){
			cerr<< "File could not be opened\n";
			exit(1);
		}

	}
	*/

private:
	const char* g_cServer;
	SOCKET Socket;
};
