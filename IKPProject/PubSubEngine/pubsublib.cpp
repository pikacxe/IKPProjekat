#pragma once
#include "pubsublib.h"

DWORD WINAPI PUBAcceptThread(LPVOID lpParam) {
	THR_ARGS* thrArgs = (THR_ARGS*)lpParam;
	HANDLE completitionPort = thrArgs->completionPort;

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


	IODATA* reqData = NULL;
	REQUEST* request = NULL;
	while (!cancelationToken) {

		// accept a client socket
		acceptSocket = accept(listenSocket, NULL, NULL);
		if (acceptSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			break;
		}
		printf("PUB client connected\n");

		if ((reqData = (IODATA*)GlobalAlloc(GPTR, sizeof(IODATA))) == NULL) {
			printf("GlobalAlloc failed\n");
			closesocket(acceptSocket);
			continue;
		}

		if ((request = (REQUEST*)GlobalAlloc(GPTR, sizeof(REQUEST))) == NULL) {
			printf("GobalAlloc failed\n");
			closesocket(acceptSocket);
			continue;
		}

		// create a new client
		request->socket = acceptSocket;
		request->type = SOCK_TYPE_PUB;
		// add the client to the completition port
		if (CreateIoCompletionPort((HANDLE)acceptSocket, completitionPort, (ULONG_PTR)request, 0) == NULL)
		{
			printf("CreateIoCompletionPort failed with error: %d\n", GetLastError());
			closesocket(acceptSocket);
			continue;
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
				closesocket(acceptSocket);
			}
		}
	}
	// thread cleanup
	printf("PubAccept thread cleanup...\n");
	closesocket(listenSocket);
	closesocket(acceptSocket);
	if (reqData != NULL) {
		GlobalFree(reqData);
	}
	if (request != NULL) {
		GlobalFree(request);
	}
	WSACleanup();
	printf("PubAccept thread cleanup done\n");
	printf("PubAccept thread exiting...\n");
	return 0;
}

DWORD WINAPI SUBAcceptThread(LPVOID lpParam) {
	THR_ARGS* thrArgs = (THR_ARGS*)lpParam;
	HANDLE completitionPort = thrArgs->completionPort;

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

	IODATA* reqData = NULL;
	REQUEST* request = NULL;
	while (!cancelationToken) {

		// accept a client socket
		acceptSocket = accept(listenSocket, NULL, NULL);
		if (acceptSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			break;
		}

		printf("SUB client connected\n");

		if ((reqData = (IODATA*)GlobalAlloc(GPTR, sizeof(IODATA))) == NULL) {
			printf("GlobalAlloc failed\n");
			continue;
		}

		if ((request = (REQUEST*)GlobalAlloc(GPTR, sizeof(REQUEST))) == NULL) {
			printf("GobalAlloc failed\n");
			continue;
		}

		// create a new client
		request->socket = acceptSocket;
		request->type = SOCK_TYPE_SUB;
		// add the client to the completition port
		if (CreateIoCompletionPort((HANDLE)acceptSocket, completitionPort, (ULONG_PTR)request, 0) == NULL)
		{
			printf("CreateIoCompletionPort failed with error: %d\n", GetLastError());
			break;
		}

		ZeroMemory(&(reqData->Overlapped), sizeof(OVERLAPPED));
		ZeroMemory(reqData->Buffer, BUFF_SIZE);
		reqData->BytesSEND = 0;
		reqData->BytesRECV = 0;
		reqData->DataBuf.len = BUFF_SIZE;
		reqData->DataBuf.buf = reqData->Buffer;

		DWORD Flags = 0;
		DWORD RecvBytes = 0;


		// receive data until the client shuts down the connection
		if (WSARecv(acceptSocket, &(reqData->DataBuf), 1, &RecvBytes, &Flags, &(reqData->Overlapped), NULL) == SOCKET_ERROR) {
			int error = WSAGetLastError();
			if (error == WSAESHUTDOWN) {
				// Connection closed gracefully
				printf("Client disconected!\n");
				closesocket(acceptSocket);
			}
			else if (error != ERROR_IO_PENDING) {
				printf("WSARecv failed with error: %d\n", error);
				closesocket(acceptSocket);
			}
		}
	}
	// thread cleanup
	printf("SubAccept thread cleanup...\n");
	closesocket(listenSocket);
	closesocket(acceptSocket);
	if (reqData != NULL) {
		GlobalFree(reqData);
	}
	if (request != NULL) {
		GlobalFree(request);
	}
	WSACleanup();
	printf("SubAccept thread cleanup done\n");
	printf("SubAccept thread exiting...\n");
	return 0;
}


void send_message_to_subs(HASH_TABLE* hashTable, char topic[], char msg[]) {
	LIST* subs = NULL;
	if ((subs = get_table_item(hashTable, topic)) != NULL) {
		if (subs->count > 0) {
			printf("Topic \"%s\" exists, sending to subscribers...\n", topic);
			LIST_ITEM* sub = subs->head;
			while (sub != NULL) {
				// send to all subscribers
				send(sub->data, msg, strlen(msg) + 1, 0);
				sub = sub->next;
			}
		}
	}
}

DWORD WINAPI WorkerThread(LPVOID args) {
	THR_ARGS* thrArgs = (THR_ARGS*)args;
	HANDLE completionPort = thrArgs->completionPort;
	HASH_TABLE* hashTable = thrArgs->hashTable;
	DWORD bytesTransferred;
	REQUEST* request;
	IODATA* ioData;
	DWORD sendBytes, recvBytes;
	DWORD Flags;
	bool keyExists;


	while (!cancelationToken) {

		if (GetQueuedCompletionStatus(completionPort, &bytesTransferred, (PULONG_PTR)&request, (LPOVERLAPPED*)&ioData, INFINITE) == 0) {
			if (GetLastError() == ERROR_ABANDONED_WAIT_0) {
				// thread was cancelled
				break;
			}
			printf("GetQueuedCompletionStatus failed with error: %d\n", GetLastError());

		}
		else {
			if (bytesTransferred == 0) {
				// client disconnected
				printf("Client disconnected\n");
				ZeroMemory(ioData->Buffer, BUFF_SIZE);
				closesocket(request->socket);
				continue;
			}
			// Pub
			else if (request->type == SOCK_TYPE_PUB) {
				PUB_INFO* pubInfo = (PUB_INFO*)ioData->Buffer;
				if (strcmp(pubInfo->msg, "exit") == 0) {
					printf("PUB client %u disconnected\n", (int)pubInfo->sock);
					send_message_to_subs(hashTable, pubInfo->topic, pubInfo->msg);
					closesocket(pubInfo->sock);
				}
				else {
					printf("PUB client %u sent for \"%s\": %s\n", (int)pubInfo->sock, pubInfo->topic, pubInfo->msg);
					keyExists = has_key(hashTable, pubInfo->topic);
					if (!keyExists) {
						// create topic
						printf("Topic \"%s\" does not exist, creating...\n", pubInfo->topic);
						if (add_list_table(hashTable, pubInfo->topic)) {
							printf("Topic \"%s\" created\n", pubInfo->topic);
						}
						else {
							printf("add_list_table failed: topic not created");
						}
					}
					else {
						send_message_to_subs(hashTable, pubInfo->topic, pubInfo->msg);
					}
					ZeroMemory(ioData->Buffer, BUFF_SIZE);
				}
			}
			// SUB
			else if (request->type == SOCK_TYPE_SUB) {
				printf("SUB client %u requesting  subscription to \"%s\"\n", (int)request->socket, ioData->Buffer);
				if (has_key(hashTable, ioData->Buffer)) {
					if (!add_table_item(hashTable, ioData->Buffer, request->socket)) {
						printf("subscribe add_list_item failed");
					}
					else
					{
						printf("SUB client %u subscribed to \"%s\"\n", (int)request->socket, ioData->Buffer);
						send(request->socket, ioData->Buffer, strlen(ioData->Buffer) + 1, 0);
					}
				}
				else {
					printf("Topic \"%s\" does not exist. Notifing client...\n", ioData->Buffer);
					sendBytes = strlen("-") + 1;
					send(request->socket, "-", sendBytes, 0);

				}
			}
			else {
				printf("Unknown client type\n");
			}

		}

		Flags = 0;
		ZeroMemory(&(ioData->Overlapped), sizeof(OVERLAPPED));
		ZeroMemory(ioData->Buffer, BUFF_SIZE);
		ioData->BytesRECV = 0;
		ioData->DataBuf.len = BUFF_SIZE;
		ioData->DataBuf.buf = ioData->Buffer;

		if (WSARecv(request->socket, &(ioData->DataBuf), 1, &recvBytes, &Flags, &(ioData->Overlapped), NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != ERROR_IO_PENDING) {
				printf("WSARecv failed with error: %d\n", WSAGetLastError());
			}
		}
	}
	printf("Worker thread finished...\n");
	return 0;

}
