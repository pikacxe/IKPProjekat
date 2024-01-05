#pragma once
#include "common.h"

bool InitializeWinsock()
{
	WSADATA wsaData;
	// Initialize Win sock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup() failed with error: %d\n", WSAGetLastError());
		return false;
	}	
	return true;
}

SOCKET connect(short port)
{
	SOCKET sock = INVALID_SOCKET;

	if (InitializeWinsock() == false)
	{
		printf("Failed to initialize winsock\n");
		return INVALID_SOCKET;
	}
	sockaddr_in addr;
	int addr_len = sizeof(addr);

	// create socket
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("socket() failed with error: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	if (connect(sock, (sockaddr*)&addr, addr_len) == SOCKET_ERROR)
	{
		printf("connect() failed with error: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	return sock;
}
