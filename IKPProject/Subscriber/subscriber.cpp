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

	// choose a topic
	do {
		printf("Enter topic: ");
		if (fgets(buffer, MAX_TOPIC_LEN, stdin) == NULL)
		{
			printf("Failed to read topic\n");
			break;
		}
		// remove newline character
		if (buffer[strlen(buffer) - 1] == '\n')
		{
			buffer[strlen(buffer) - 1] = '\0';
		}
		if (strcmp(buffer, "") == 0)
		{
			continue;
		}
		if (strcmp(buffer, "exit") == 0)
		{
			shutdown(sock, SD_BOTH);
			closesocket(sock);
			WSACleanup();
			return 0;
		}
		if (send(sock, buffer, BUFF_SIZE, 0) == SOCKET_ERROR)
		{
			printf("send() failed with error: %d\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			return 1;
		}
		printf("Requesting subscription to topic \"%s\"...\n", buffer);	
		if (recv(sock, buffer, BUFF_SIZE, 0) == SOCKET_ERROR)
		{
			printf("recv() failed with error: %d\n", WSAGetLastError());
			break;
		}
		if (strcmp(buffer, "-") == 0)
		{
			printf("Topic does not exist\n");
		}
		else
		{
			break;
		}

	} while (1);


	printf("Subscribed to topic \"%s\"\n", buffer);
	printf("Receiving messages...\n\n");
	// receive messages
	while (1) {
		memset(buffer, 0, BUFF_SIZE);
		if (recv(sock, buffer, BUFF_SIZE, 0) == SOCKET_ERROR)
		{
			printf("recv() failed with error: %d\n", WSAGetLastError());
			break;
		}
		if (strcmp(buffer, "-") == 0)
		{
			printf("Topic does not exist\n");
			printf("Enter topic: ");
			if (fgets(buffer, MAX_TOPIC_LEN, stdin) == NULL)
			{
				printf("Failed to read topic\n");
				break;
			}
			// remove newline character
			if (buffer[strlen(buffer) - 1] == '\n')
			{
				buffer[strlen(buffer) - 1] = '\0';
			}
			//send new topic
			if (send(sock, buffer, BUFF_SIZE, 0) == SOCKET_ERROR)
			{
				printf("send() failed with error: %d\n", WSAGetLastError());
				closesocket(sock);
				WSACleanup();
				break;
			}
		}
		// check if message is "exit"
		else if (strcmp(buffer, "exit") == 0)
		{
			break;
		}
		else
		{
			printf("Message received: %s\n", buffer);
		}
	}
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	WSACleanup();
	printf("Press any key to exit...\n");
	getchar();
	return 0;

}