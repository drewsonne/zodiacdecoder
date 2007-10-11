#include "ZodiacClient.h"
#include <iostream>
using namespace std;

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
		printf("WSA Initialization failed.");
		return false;
	}
	return true;

}
bool ZodiacClient::setServer(char* server)
{
	g_cServer = server;
	if (!initializeClient())
		return false;
	return true;

}
bool ZodiacClient::Connect(int ip_b1, int ip_b2, int ip_b3, int ip_b4, int port)
{
	if(!initializeClient())
		return false;
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	if(Socket == INVALID_SOCKET)
	{
		printf("Socket creation failed.");
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
		printf("Failed to establish connection with server.");
		return false;  
	}
	return true;

}
bool ZodiacClient::sendKey(char* key, int key_size)
{	
	int RetVal = SOCKET_ERROR;
	char* String = key;
	RetVal = send(Socket, String, key_size, 0);
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

int main(){

	printf("Sending Hello World!\n");
	ZodiacClient* fo_sho = new ZodiacClient();
	fo_sho->Connect(127,0,0,1,10000);
	fo_sho->sendKey("Hello World!\0", 14);
	fo_sho->slayClient();
	printf("I think it was sent!\n");
}