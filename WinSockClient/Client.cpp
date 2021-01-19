#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 27016
#define OUTGOING_BUFFER_SIZE 1024
#define SLEEP_TIME 500

#define SERVER_IP_ADDERESS "127.0.0.1"

// Initializes WinSock2 library
// Returns true if succeeded, false otherwise.
bool InitializeWindowsSockets();
int Send(SOCKET connectSocket, char* messageToSend, int len);
void Provera(SOCKET acceptedSocket);

int __cdecl main(int argc, char **argv) 
{
    // socket used to communicate with server
    SOCKET connectSocket = INVALID_SOCKET;
    // variable used to store function return value
    int iResult;
    // message to send
	char messageToSend[OUTGOING_BUFFER_SIZE];
    
    // Validate the parameters
	/*
    if (argc != 2)
    {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }
	*/

    if(InitializeWindowsSockets() == false)
    {
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
    }

    // create a socket
    connectSocket = socket(AF_INET,
                           SOCK_STREAM,
                           IPPROTO_TCP);

    if (connectSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // create and initialize address structure
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP_ADDERESS);
    serverAddress.sin_port = htons(DEFAULT_PORT);
    // connect to server specified in serverAddress and socket connectSocket
    if (connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        printf("Unable to connect to server.\n");
        closesocket(connectSocket);
        WSACleanup();
    }

	while (true)
	{
		printf("Enter message from server:\n");

		// Read string from user into outgoing buffer
		gets_s(messageToSend, OUTGOING_BUFFER_SIZE);

		// Send an prepared message with null terminator included
		//iResult = send(connectSocket, messageToSend, (int)strlen(messageToSend) + 1, 0);
		char b[4];
		itoa((int)strlen(messageToSend) + 1, b, 10);

		iResult = Send(connectSocket, b, 4);

		iResult = Send(connectSocket, messageToSend, (int)strlen(messageToSend) + 1);

		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(connectSocket);
			WSACleanup();
			return 1;
		}

		printf("Bytes Sent: %ld\n", iResult);
	}

    // cleanup
    closesocket(connectSocket);
    WSACleanup();

    return 0;
}

void Provera(SOCKET socket)
{
	while (1)
	{
		int iResult;
		FD_SET set;
		timeval timeVal;

		FD_ZERO(&set);
		// Add socket we will wait to read from
		FD_SET(socket, &set);

		// Set timeouts to zero since we want select to return
		// instantaneously
		timeVal.tv_sec = 0;
		timeVal.tv_usec = 0;

		iResult = select(0 /* ignored */, &set, &set, NULL, &timeVal);

		// lets check if there was an error during select
		if (iResult == SOCKET_ERROR)
		{
			fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());
			return;
		}

		// now, lets check if there are any sockets ready
		if (iResult == 0)
		{
			//printf("Nema poruka\n");
			// there are no ready sockets, sleep for a while and check again
			Sleep(SLEEP_TIME);
			continue;
		}
		break;
	}
}

int Send(SOCKET connectSocket, char* messageToSend, int len)
{
	int brojac = 0;

	while (brojac < len) {
		Provera(connectSocket);
		int res = send(connectSocket, messageToSend + brojac, len - brojac, 0);
		if (res > 0) {
			brojac += res;
		}
		
	}

	return brojac;
}

bool InitializeWindowsSockets()
{
    WSADATA wsaData;
	// Initialize windows sockets library for this process
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return false;
    }
	return true;
}
