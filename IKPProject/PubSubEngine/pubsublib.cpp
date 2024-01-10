#pragma once
#include "pubsublib.h"



DWORD WINAPI PUBAcceptThread(LPVOID lpParam) {
	HANDLE completitionPort = (HANDLE)lpParam;

	char* buffer = (char*)malloc(BUFF_SIZE);
	if (buffer == NULL)
	{
		printf("malloc failed with error: %d\n", GetLastError());
		return 1;
	}

	// information structures for the overlapped I/O operations
	addrinfo* resultingAddress = NULL;
	addrinfo hints;

	int iResult;

	SOCKET acceptSocket = INVALID_SOCKET;
	SOCKET listenSocket = INVALID_SOCKET;

	// set up the hints structure
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_protocol = IPPROTO_TCP; // TCP
	hints.ai_flags = AI_PASSIVE; // server

	// resolve the server address and port
	iResult = getaddrinfo(NULL, PUB_PORT_S, &hints, &resultingAddress);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// create listen socket
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(resultingAddress);
		WSACleanup();
		return 1;
	}

	iResult = bind(listenSocket, resultingAddress->ai_addr, (int)resultingAddress->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(resultingAddress);
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// free unused structs
	freeaddrinfo(resultingAddress);

	// start listening on the socket
	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	printf("PUB server is listening on port %s\n", PUB_PORT_S);

	while (true) {
		// create request
		REQUEST* request = NULL;
		// accept a client socket
		acceptSocket = accept(listenSocket, NULL, NULL);
		if (acceptSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		printf("PUB client connected\n");

		if ((request = (REQUEST*)GlobalAlloc(GPTR, sizeof(REQUEST))) == NULL)
		{
			printf("GlobalAlloc failed with error: %d\n", GetLastError());
			closesocket(listenSocket);
			closesocket(acceptSocket);
			WSACleanup();
			return 1;
		}

		// create a new client
		request->socket = acceptSocket;
		request->type = SOCK_TYPE_PUB;
		// add the client to the completition port
		if (CreateIoCompletionPort((HANDLE)acceptSocket, completitionPort, (ULONG_PTR)request, 0) == NULL)
		{
			printf("CreateIoCompletionPort failed with error: %d\n", GetLastError());
			closesocket(listenSocket);
			closesocket(acceptSocket);
			GlobalFree(request);
			WSACleanup();
			return 1;
		}

		IODATA* reqData = NULL;

		if ((reqData = (IODATA*)GlobalAlloc(GPTR, sizeof(IODATA))) == NULL)
		{
			printf("GlobalAlloc failed with error: %d\n", GetLastError());
			closesocket(listenSocket);
			closesocket(acceptSocket);
			GlobalFree(request);
			WSACleanup();
			return 1;
		}

		ZeroMemory(&(reqData->Overlapped), sizeof(OVERLAPPED));
		reqData->BytesRECV = 0;
		reqData->BytesSEND = 0;
		reqData->DataBuf.len = BUFF_SIZE;
		reqData->DataBuf.buf = reqData->Buffer;

		DWORD Flags = 0;
		DWORD RecvBytes = 0;

		// receive data until the client shuts down the connection
		if (WSARecv(acceptSocket, &(reqData->DataBuf), 1, &RecvBytes, &Flags, &(reqData->Overlapped), NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != ERROR_IO_PENDING) {
				printf("WSARecv failed with error: %d\n", WSAGetLastError());
				closesocket(listenSocket);
				closesocket(acceptSocket);
				GlobalFree(request);
				GlobalFree(reqData);
				WSACleanup();
				return 1;
			}
		}
	}
}

DWORD WINAPI SUBAcceptThread(LPVOID lpParam) {
	HANDLE completitionPort = (HANDLE)lpParam;

	char* buffer = (char*)malloc(BUFF_SIZE);
	if (buffer == NULL) {
		printf("malloc failed with error: %d\n", GetLastError());
		return 1;
	}

	// information structures for the overlapped I/O operations
	addrinfo* resultingAddress = NULL;
	addrinfo hints;

	int iResult;

	SOCKET acceptSocket = INVALID_SOCKET;
	SOCKET listenSocket = INVALID_SOCKET;

	// set up the hints structure
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_protocol = IPPROTO_TCP; // TCP
	hints.ai_flags = AI_PASSIVE; // server

	// resolve the server address and port
	iResult = getaddrinfo(NULL, SUB_PORT_S, &hints, &resultingAddress);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// create listen socket
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(resultingAddress);
		WSACleanup();
		return 1;
	}

	iResult = bind(listenSocket, resultingAddress->ai_addr, (int)resultingAddress->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(resultingAddress);
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// free unused structs
	freeaddrinfo(resultingAddress);

	// start listening on the socket
	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	printf("SUB server is listening on port %s\n", SUB_PORT_S);

	while (true) {
		// create request
		REQUEST* request = NULL;
		// accept a client socket
		acceptSocket = accept(listenSocket, NULL, NULL);
		if (acceptSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		printf("SUB client connected\n");

		if ((request = (REQUEST*)GlobalAlloc(GPTR, sizeof(REQUEST))) == NULL)
		{
			printf("GlobalAlloc failed with error: %d\n", GetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}

		// create a new client
		request->socket = acceptSocket;
		request->type = SOCK_TYPE_SUB;
		// add the client to the completition port
		if (CreateIoCompletionPort((HANDLE)acceptSocket, completitionPort, (ULONG_PTR)request, 0) == NULL)
		{
			printf("CreateIoCompletionPort failed with error: %d\n", GetLastError());
			closesocket(listenSocket);
			closesocket(acceptSocket);
			GlobalFree(request);
			WSACleanup();
			return 1;
		}

		IODATA* reqData = NULL;

		if ((reqData = (IODATA*)GlobalAlloc(GPTR, sizeof(IODATA))) == NULL)
		{
			printf("GlobalAlloc failed with error: %d\n", GetLastError());
			closesocket(listenSocket);
			closesocket(acceptSocket);
			GlobalFree(request);
			WSACleanup();
			return 1;
		}

		ZeroMemory(&(reqData->Overlapped), sizeof(OVERLAPPED));
		reqData->BytesRECV = 0;
		reqData->BytesSEND = 0;
		reqData->DataBuf.len = BUFF_SIZE;
		reqData->DataBuf.buf = reqData->Buffer;

		DWORD Flags = 0;
		DWORD RecvBytes = 0;

		// receive data until the client shuts down the connection
		if (WSARecv(acceptSocket, &(reqData->DataBuf), 1, &RecvBytes, &Flags, &(reqData->Overlapped), NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != ERROR_IO_PENDING) {
				printf("WSARecv failed with error: %d\n", WSAGetLastError());
				closesocket(listenSocket);
				closesocket(acceptSocket);
				GlobalFree(request);
				GlobalFree(reqData);
				WSACleanup();
				return 1;
			}
		}
	}
}

DWORD WINAPI WorkerThread(LPVOID args) {
	HANDLE completionPort = ((THR_ARGS*)args)->completionPort;
	HASH_TABLE* hashTable = ((THR_ARGS*)args)->hashTable;
	DWORD bytesTransferred;
	REQUEST* request;
	IODATA* ioData;
	DWORD sendBytes, recvBytes;
	DWORD Flags;
	LIST* subs;
	bool keyExists;


	while (true) {
		if (GetQueuedCompletionStatus(completionPort, &bytesTransferred, (PULONG_PTR)&request, (LPOVERLAPPED*)&ioData, INFINITE) == 0) {
			printf("GetQueuedCompletionStatus failed with error: %d\n", GetLastError());
			return 1;
		}

		// Pub
		if (request->type == SOCK_TYPE_PUB) {
			PUB_INFO* pubInfo = (PUB_INFO*)ioData->Buffer;
			if (strcmp(pubInfo->topic, "exit") == 0) {
				printf("PUB client %u disconnected\n", request->socket);
				shutdown(request->socket, SD_BOTH);
				closesocket(request->socket);
				GlobalFree(request);
				GlobalFree(ioData);
				continue;
			}
			printf("PUB client %u sent for \"%s\": %s\n", pubInfo->sock, pubInfo->topic, pubInfo->msg);
			keyExists = has_key(hashTable, pubInfo->topic);
			if (!keyExists) {
				// create topic
				printf("Topic \"%s\" does not exist, creating...\n", pubInfo->topic);
				if (!add_list_table(hashTable, pubInfo->topic)) {
					printf("add_list_table failed with error: %d\n", GetLastError());
					return 1;
				}
				else {
					printf("Topic \"%s\" created\n", pubInfo->topic);
				}
			}
			else {
				if ((subs = get_table_item(hashTable, pubInfo->topic)) == NULL) {
					printf("get_table_item failed with error: %d\n", GetLastError());
					return 1;
				}
				else {
					printf("Topic \"%s\" exists, sending to subscribers...\n", pubInfo->topic);
					LIST_ITEM* sub = subs->head;
					while (sub != NULL) {
						// send to all subscribers
						Flags = 0;
						recvBytes = 0;
						sendBytes = strlen(pubInfo->msg) + 1;
						ioData->BytesRECV = 0;
						ioData->DataBuf.len = sendBytes;
						ioData->DataBuf.buf = pubInfo->msg;
						if (send(sub->data, pubInfo->msg, sendBytes, 0) == SOCKET_ERROR) {
							if (WSAGetLastError() != ERROR_IO_PENDING) {
								printf("WSASend failed with error: %d\n", WSAGetLastError());
								return 1;
							}
						}
						sub = sub->next;
					}
				}
			}
		}
		// SUB
		else if (request->type == SOCK_TYPE_SUB) {
			if (strcmp(ioData->Buffer, "") == 0) {
				continue;
			}
			if (strcmp(ioData->Buffer, "exit") == 0) {
				printf("SUB client %u disconnected\n", request->socket);
				shutdown(request->socket, SD_BOTH);
				closesocket(request->socket);
				GlobalFree(request);
				GlobalFree(ioData);
				continue;
			}
			if (strcmp(ioData->Buffer, "?") == 0) {
				// send available topics
				char* topics = get_table_keys(hashTable);
				if (topics == NULL) {
					printf("get_keys failed with error: %d\n", GetLastError());
					return 1;
				}
				else {
					ZeroMemory(&(ioData->Overlapped), sizeof(OVERLAPPED));
					ioData->BytesRECV = 0;
					ioData->DataBuf.len = BUFF_SIZE;
					ioData->DataBuf.buf = topics;

					if (WSASend(request->socket, &(ioData->DataBuf), 1, &sendBytes, 0, &(ioData->Overlapped), NULL) == SOCKET_ERROR) {
						if (WSAGetLastError() != ERROR_IO_PENDING) {
							printf("WSASend failed with error: %d\n", WSAGetLastError());
							return 1;
						}
					}
					continue;
				}
			}
			printf("SUB client %u requesting  subscription to \"%s\"\n", request->socket, ioData->Buffer);
			if (has_key(hashTable, ioData->Buffer)) {
				if (!add_table_item(hashTable, ioData->Buffer, request->socket)) {
					printf("subscribe add_list_item failed with error: %d\n", GetLastError());
					return 1;
				}
				else
				{
					printf("SUB client %u subscribed to \"%s\"\n", request->socket, ioData->Buffer);
				}
			}
			else {
				printf("Topic \"%s\" does not exist. Notifing client...\n", ioData->Buffer);
				ZeroMemory(&(ioData->Overlapped), sizeof(OVERLAPPED));
				ioData->BytesRECV = 0;
				ioData->DataBuf.len = 1;
				strcpy_s(ioData->DataBuf.buf, 1, "");

				if (WSASend(request->socket, &(ioData->DataBuf), 1, &sendBytes, 0, &(ioData->Overlapped), NULL) == SOCKET_ERROR) {
					if (WSAGetLastError() != ERROR_IO_PENDING) {
						printf("WSASend failed with error: %d\n", WSAGetLastError());
						return 1;
					}
				}
				continue;
			}
		}
		else {
			printf("Unknown client type\n");
		}

		Flags = 0;
		ZeroMemory(&(ioData->Overlapped), sizeof(OVERLAPPED));
		ioData->BytesRECV = 0;
		ioData->DataBuf.len = BUFF_SIZE;
		ioData->DataBuf.buf = ioData->Buffer;

		if (WSARecv(request->socket, &(ioData->DataBuf), 1, &recvBytes, &Flags, &(ioData->Overlapped), NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != ERROR_IO_PENDING) {
				printf("WSARecv failed with error: %d\n", WSAGetLastError());
				return 1;
			}
		}
	}

}


