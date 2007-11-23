
#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
//#pragma once
//due to some problems with windows.h including winsock.h, comment out the line #include <winsock.h> in windows.h please
//That will allow this code to compile. Sorry for any inconveniences with that, but I'm unsure if there's an undefine or
//or exclude command to prevent winsock.h from being loaded. Anyway, that's what needs to be done with this one to get it
//to compile.
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib


//Need to figure out the DEFAULT_BUFLEN still. umm. I think it would be 63 or 64 by default.
//If I remember right, it does not count the null terminator. So, 63 should be fine.
//#define NUM_KEYS 10
#define DEFAULT_BUFLEN 64
#define DEFAULT_PORT "10001"

//keys is a 2-dimensional array passed by reference
//please make sure what you make is an array like so [10][63] plz.
//If it doesn't like it, plz cast it to (char **&). kthxbye

int getKeysFromServer(char* keys) 
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char *sendbuf = "9";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN * 1;
// Server name goes here!
    char *serverName="zodiacdecoder.dyndns.org";
    

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(serverName, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);
	//printf("%c", sendbuf[0]);

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {
        // this should go until server quits sending us some cipher keys.
        // should be fairly straight forward. The server would just send all of the keys in the file
        // still need to figure out what to do with the keys. Should we use a stack, a linked list, or just an array
        // and have the server send a number stating the number of keys it will be sending. Should be fairly straight
        // forward though.
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		//printf("%s %d", recvbuf,63);
		if ( iResult > 0 ){
			for(int i = 0; i < 64; i++){
				keys[i] = recvbuf[i];
			}
		}
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());

	} while (false);
	//while( iResult > 0 );

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
int sendToServer(char* server, char* port, char* sendbuf)
{
	char* sendVoteBuf = new char[64]();
	for(int i = 0; i < 64; i++)
		sendVoteBuf[i] = sendbuf[i];

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char recvbuf[DEFAULT_BUFLEN + 1];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN + 1;
    

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(server, port, &hints, &result);
    if ( iResult != 0 ) {
		printf("%s\n", server);
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send the key to server.
    iResult = send( ConnectSocket, sendVoteBuf, 64, 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);
/*	for(int i = 0; i<64; i++)
		printf("%c", sendVoteBuf[i]);
	printf("\n");
	for(int i = 64; i < 73; i++)
		printf("%c", sendVoteBuf[i]);
	printf("\n");
*/
    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

/*   // Receive until the peer closes the connection
     // Not needed yet. We should only need to send that single vote.
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
            printf("Bytes received: %d\n", iResult);
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());

    } while( iResult > 0 );

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
*/
    return 0;
}
