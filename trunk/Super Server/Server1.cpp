/*#include "Server.h"

int main()
{
	unsigned long j = 0;
	ifstream fin;
	fin.open("numkeysvoted.txt", ios::in);
	fin >> j;
	fin.close();
	time_t seconds;
	while(true){
		if(serve("goodkeys.txt", "10002", j))
			printf("I'm a naughty, naughty boy");
		else
		{
			seconds = time(NULL);
			FILE* cfile  = fopen("times.txt","a");
			fprintf(cfile, "%ld\n", seconds);
			fclose(cfile);
		}
	}
	return 0;
}*/
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