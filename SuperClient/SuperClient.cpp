#include "Client.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main()
{
	char key[63];
	ifstream fin;
	fin.open("goodkey.txt", ios::in);
	fin >> key;

	if(sendToServer("zodiacdecoder.dyndns.org","10002",key))
	{
		fin.close();
		return 1;
	}
	fin.close();
	return 0;
}

/*int main(){
	
	char allo[63];
	for(int j = 0; j<63; j++){
			allo[j] = 'a';
			//printf("%c", allo[j]);
	
	}
	fstream fout;
	fout.open("superclientkeys.txt", ios::out);
	if(!getKeysFromServer(allo)){
		for(int i = 0; i < 63; i++){
			fout << allo[i];
		}
	}else
		printf("You're a tard");
	fout.close();
	

  return 0;
}
*/
/*
int main(){
	char allo[63];
	fin 
	sendToServer("zodiacdecoder.dyndns.org", "10000", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabb");
	return 0;
}*/
