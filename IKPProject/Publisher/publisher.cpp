#include "../Common/common.h"

int main()
{
	printf("Preparing publisher...\n\n");

	SOCKET sock = connect(PUB_PORT);
	if (sock == INVALID_SOCKET)
	{
		printf("Failed to connect to server\n");
		return 1;
	}

	printf("Connected to server\n");

	char buffer[BUFF_SIZE];
	memset(buffer, 0, BUFF_SIZE);
	PUB_INFO pi;

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
		if (fgets(pi.topic, MAX_TOPIC_LEN, stdin) == NULL)
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
				if (strcmp(pi.topic, available_topics.topics[i]) == 0)
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

	//cleanup topics
	if (available_topics.count > 0) {
		for (int i = 0; i < available_topics.count; i++)
		{
			free(available_topics.topics[i]);
		}
		free(available_topics.topics);
	}

	// trim newline character
	if (pi.topic[strlen(pi.topic) - 1] == '\n')
	{
		pi.topic[strlen(pi.topic) - 1] = '\0';
	}

	// start publishing messages to chosen topic
	while (true) {
		printf("Enter message for \"%s\": ", pi.topic);
		if (fgets(pi.msg, MAX_MSG_LEN, stdin) == NULL)
		{
			printf("fgets() failed: message\n");
			break;
		}

		// trim newline character
		if (pi.msg[strlen(pi.msg) - 1] == '\n')
		{
			pi.msg[strlen(pi.msg) - 1] = '\0';
		}

		if (send(sock, (char*)&pi, sizeof(PUB_INFO), 0) == SOCKET_ERROR)
		{
			printf("send() failed with error: %d\n", WSAGetLastError());
			break;
		}
		if (strcmp(pi.msg, "exit") == 0)
		{
			break;
		}
		printf("Message sent\n");
	}
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	WSACleanup();

	printf("Press any key to continue...\n");
	getchar();
	return 0;
}