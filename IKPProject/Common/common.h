#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdio.h>


#define SERVER_ADDR "127.0.0.1"
#define PUB_PORT 10000
#define SUB_PORT 10001
#define BUFF_SIZE 4096
#define MAX_TOPIC_LEN 128
#define MAX_MSG_LEN 2048

#define MAX_PUBS 10
#define MAX_SUBS 10

typedef struct _PUB_INFO
{
	char topic[MAX_TOPIC_LEN];
	char msg[MAX_MSG_LEN];
	SOCKET sock;
}PUB_INFO;

typedef struct _SUB_INFO
{
	char topic[MAX_TOPIC_LEN];
	SOCKET sock;
}SUB_INFO;



/// <summary>
/// connect to PubSub engine
/// </summary>
/// <param name="port"> - Connect port</param>
/// <param name="server_address"> - server address</param>
/// <returns>Socket handle if successful, otherwise INVALID_SOCKET</returns>
SOCKET connect(short port);


/// <summary>///
/// Initialize Winsock library
/// </summary>///
///  <returns>True if successfult, otherwise false</returns>
bool InitializeWinsock();