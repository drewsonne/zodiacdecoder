#pragma once

#include<winsock2.h>
#include <winsock2.h>
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>
#include <iostream>


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
private:
	const char* g_cServer;
	SOCKET Socket;
};
