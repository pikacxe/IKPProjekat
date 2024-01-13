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

	// get available topics

	if (recv(sock, buffer, BUFF_SIZE, 0) == SOCKET_ERROR)
	{
		printf("recv() failed with error: %d\n", WSAGetLastError());
		shutdown(sock, SD_BOTH);
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	TOPIC_INFO available_topics;
	memcpy(&available_topics, buffer, sizeof(TOPIC_INFO));

	do {
		printf("Available topics:\n");
		if (available_topics.count == 0)
		{
			printf("No topics available\n");
		}
		else if (available_topics.count > 0) {
			for (int i = 0; i < available_topics.count; i++)
			{
				printf("%s\n", available_topics.topics[i]);
			}
		}
		printf("Enter topic: ");
		if (fgets(buffer, MAX_TOPIC_LEN, stdin) == NULL)
		{
			printf("fgets() failed: topic\n");
			shutdown(sock, SD_BOTH);
			closesocket(sock);
			WSACleanup();
			return 1;
		}
		if (available_topics.count > 0) {

			for (int i = 0; i < available_topics.count; i++)
			{
				if (strcmp(buffer, available_topics.topics[i]) == 0)
				{
					break;
				}
				else if (i == available_topics.count - 1)
				{
					printf("Topic does not exist\n");
					continue;
				}
			}
		}
		else {
			break;
		}
	} while (1);

	// clean up topics 
	if (available_topics.count > 0) {
		for (int i = 0; i < available_topics.count; i++)
		{
			free(available_topics.topics[i]);
		}
		free(available_topics.topics);
	}

	// remove newline character
	if (buffer[strlen(buffer) - 1] == '\n')
	{
		buffer[strlen(buffer) - 1] = '\0';
	}

	if (send(sock, buffer, BUFF_SIZE, 0) == SOCKET_ERROR)
	{
		printf("send() failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	printf("Subscribed to topic \"%s\"\n", buffer);
	printf("Receiving messages...\n\n");	
	// receive messages
	while (true) {
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