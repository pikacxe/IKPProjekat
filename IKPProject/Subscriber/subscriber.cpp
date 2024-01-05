#include "../Common/common.h"

int main()
{
	printf("Preparing subscriber...\n\n");

	SOCKET sock = connect(SUB_PORT);
	if (sock == INVALID_SOCKET)
	{
		printf("Failed to connect to server\n");
		return 1;
	}

	printf("Connected to server...\n");

	char buffer[BUFF_SIZE];
	memset(buffer, 0, BUFF_SIZE);

	// receive welcome message
	if (recv(sock, buffer, BUFF_SIZE, 0) == SOCKET_ERROR)
	{
		printf("recv() failed with error: %d\n", WSAGetLastError());
		return 1;
	}
	else
	{
		// print available topics
		printf("Available topics: \n%s\n", buffer);
	}
	char* topics = buffer;

	// choose topic
	SUB_INFO si;
	char topic[MAX_TOPIC_LEN];
	while (true)
	{
		printf("Enter topic: ");
		if (fgets(topic, MAX_TOPIC_LEN, stdin) == NULL)
		{
			printf("fgets() failed: topic\n");
			closesocket(sock);
			WSACleanup();
			return 1;
		}
		// trim newline character
		if (topic[strlen(topic) - 1] == '\n')
		{
			topic[strlen(topic) - 1] = '\0';
		}
		// check if topic is available
		if (strstr(topics, topic) != NULL)
		{
			break;
		}
		else
		{
			printf("Topic \"%s\" is not available\n", topic);
		}
	}
	// subscribe to topic
	strcpy_s(si.topic, MAX_TOPIC_LEN, topic);
	if (send(sock, (char*)&si, sizeof(SUB_INFO), 0) == SOCKET_ERROR)
	{
		printf("send() failed with error: %d\n", WSAGetLastError());
		return 1;
	}


	// receive messages
	while (true) {
		memset(buffer, 0, BUFF_SIZE);
		if (recv(sock, buffer, BUFF_SIZE, 0) == SOCKET_ERROR)
		{
			printf("recv() failed with error: %d\n", WSAGetLastError());
			return 1;
		}
		else
		{
			printf("Message received: %s\n", buffer);
		}
		// check if message is "exit"
		if (strcmp(buffer, "exit") == 0)
		{
			break;
		}
	}

	closesocket(sock);
	WSACleanup();
	printf("Press any key to exit...\n");
	getchar();
	return 0;

}