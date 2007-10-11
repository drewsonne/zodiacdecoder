// Console App.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../ZodiacSocket/ZodiacClient.h"

int _tmain(int argc, _TCHAR* argv[])
{
	printf("Hello World!");
	ZodiacClient* zclient = new ZodiacClient();
	zclient->Connect(127,0,0,1,1666);
	while(true){

		zclient->sendKey("Hello World!\0", 14);
	}
	zclient->slayClient();
	delete zclient;
	zclient = NULL;
	return 0;
}

