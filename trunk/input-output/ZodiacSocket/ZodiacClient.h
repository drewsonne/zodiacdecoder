#pragma once

#include<winsock2.h>
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

ZodiacClient::ZodiacClient(void)
{
}

ZodiacClient::~ZodiacClient(void)
{
	slayClient();
}
bool ZodiacClient::initializeClient()
{
WSADATA WsaDat;
	if(WSAStartup(MAKEWORD(1,1), &WsaDat) != 0)
	{
		//WSA Initialization failed.
		return false;
	}
	return true;

}
bool ZodiacClient::setServer(char* server){return false;}

bool ZodiacClient::Connect(int ip_b1, int ip_b2, int ip_b3, int ip_b4, int port)
{
	if(!initializeClient())
		return false;
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	if(Socket == INVALID_SOCKET)
	{
//		printf("Socket creation failed.");
		return false;
	}
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = port;
	SockAddr.sin_family = AF_INET;

	SockAddr.sin_addr.S_un.S_un_b.s_b1 = ip_b1;
	SockAddr.sin_addr.S_un.S_un_b.s_b2 = ip_b2;
	SockAddr.sin_addr.S_un.S_un_b.s_b3 = ip_b3;
	SockAddr.sin_addr.S_un.S_un_b.s_b4 = ip_b4;


	if (connect(Socket, (SOCKADDR *)(&SockAddr), sizeof(SockAddr)) != 0)
    {
//		printf("Failed to establish connection with server.");
		return false;  
	}
	return true;

}
bool ZodiacClient::sendKey(char* key, int key_size)
{	
	int RetVal;
	RetVal = send(Socket, key, key_size, 0);
	printf("%d\n", RetVal);
	if((RetVal == 0) || (RetVal == WSAECONNRESET))
	{
		printf("Connection closed at other end.");
		return false;
	}
	return true;
}
void ZodiacClient::slayClient()
{
	closesocket(Socket);
}

