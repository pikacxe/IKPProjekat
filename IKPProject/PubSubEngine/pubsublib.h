#pragma once

#include "../Common/common.h"
#include "../Common/hashtable.h"
#include <WS2tcpip.h>

enum SockType {
	SOCK_TYPE_PUB,
	SOCK_TYPE_SUB
};

typedef struct _REQUEST {
	SOCKET socket;
	SockType type;
}REQUEST;

typedef struct _IODATA {
	OVERLAPPED Overlapped;
	WSABUF DataBuf;
	CHAR Buffer[BUFF_SIZE];
	DWORD BytesSEND;
	DWORD BytesRECV;
}IODATA;

typedef struct {
	HANDLE completionPort;
	HASH_TABLE* hashTable;
}THR_ARGS;


DWORD WINAPI WorkerThread(LPVOID arg);
DWORD WINAPI PUBAcceptThread(LPVOID completionPort);
DWORD WINAPI SUBAcceptThread(LPVOID completionPort);