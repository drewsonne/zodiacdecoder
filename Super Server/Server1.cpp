#include "Server.h"

int main()
{
	unsigned long j = 0;
	ifstream fin;
	fin.open("numkeys.txt", ios::in);
	fin >> j;
	fin.close();
	while(true){
		if(serve("keys.txt", "10000", j))
			printf("I'm a naughty, naughty boy");	
	}
	return 0;
}
/*#include "Server.h"


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
}*/