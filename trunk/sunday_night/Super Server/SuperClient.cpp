#include "Client.h"

int main(){
	char allo[10][63];
	if(!getKeysFromServer((char **&)allo)){
		for(int i = 0; i < 10; i++)
			printf("%s\n", allo[i]);
	}else
		printf("You're a tard");
	if(!vote("64.233.167.99", "000000000000000000000000000000000000000000000000000000000000000", FALSE))
		printf("Hallo!");
	else
		printf("Wow, you really suck!");
  return 0;
}