#include "../Common/common.h"

int main()
{
	printf("Preparing subscriber...\n\n");

	if (!InitializeWinsock())
	{
		printf("Failed to initialize winsock\n");
		return 1;
	}

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
		printf("Server: %s\n", buffer);
	}
	char* topics = buffer;
	// print available topics
	printf("Available topics: \n%s\n", topics);

	// choose topic
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
	}

	closesocket(sock);
	WSACleanup();
	printf("Press any key to exit...\n");
	getchar();
	return 0;

}