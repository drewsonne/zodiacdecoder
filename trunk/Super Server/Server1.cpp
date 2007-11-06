#include "Server.h"

int main()
{
	unsigned long j = 0;
	ifstream fin;
	fin.open("numkeys.txt", ios::in);
	fin >> j;
	fin.close();
	while(true){
		if(serve("goodkeys.txt", "10002", j))
			printf("I'm a naughty, naughty boy");	
	}
	return 0;
}