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

/// <summary>
/// Worker thread function
/// </summary>
/// <param name="arg">Thread args</param>
/// <returns>Thread handle</returns>
DWORD WINAPI WorkerThread(LPVOID arg);

/// <summary>
/// Accept thread function
/// </summary>
/// <param name="completionPort">Completion port</param>
/// <returns>Thread handle</returns>
DWORD WINAPI PUBAcceptThread(LPVOID completionPort);

/// <summary>
/// Accept thread function
/// </summary>
/// <param name="completionPort">Completion port</param>
/// <returns>Thread handle</returns>
DWORD WINAPI SUBAcceptThread(LPVOID completionPort);