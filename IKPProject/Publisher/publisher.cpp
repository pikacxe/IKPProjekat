#include <stdio.h>
#include "../Common/common.h"


int main()
{
	printf("Preparing publisher...\n\n");

	if (!InitializeWinsock())
	{
		printf("Failed to initialize winsock\n");
		return 1;
	}

	SOCKET sock = connect(PUB_PORT);
	if (sock == INVALID_SOCKET)
	{
		printf("Failed to connect to server\n");
		return 1;
	}

	printf("Connected to server\n");

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

	PUB_INFO pi;
	// start publishing messages to chosen topic
	char topic[MAX_TOPIC_LEN];
	printf("Enter topic: ");
	if (fgets(pi.topic, MAX_TOPIC_LEN, stdin) == NULL)
	{
		printf("fgets() failed: topic\n");
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	// trim newline character
	if (pi.topic[strlen(pi.topic) - 1] == '\n')
	{
		pi.topic[strlen(pi.topic) - 1] = '\0';
	}
	while (true) {


		printf("Enter message for \"%s\": ", topic);
		if (fgets(pi.msg, MAX_MSG_LEN, stdin) == NULL)
		{
			printf("fgets() failed: message\n");
			closesocket(sock);
			WSACleanup();
			return 1;
		}

		// trim newline character
		if (pi.msg[strlen(pi.msg) - 1] == '\n')
		{
			pi.msg[strlen(pi.msg) - 1] = '\0';
		}

		if (send(sock, (char*)&pi, sizeof(PUB_INFO), 0) == SOCKET_ERROR)
		{
			printf("send() failed with error: %d\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			return 1;
		}
		printf("Message sent\n");
	}
	closesocket(sock);
	WSACleanup();
	printf("Press any key to continue...\n");
	getchar();
	return 0;
}