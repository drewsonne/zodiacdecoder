#include "Server.h"


int main(){
	ofstream fout;
	unsigned long j = 0;
	ifstream fin;
	fin.open("number_of_keys_received.dat", ios::in);
	fin >> j;
	fin.close();

	while(true)
	{
		if(!serveKeys("keys.txt", "10001", j))
		{
			fout.open("number_of_keys_received.dat", ios::out);
			j++;
			fout << j;
			fout.close();
		}

	}
	return 0;
}