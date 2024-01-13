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

	while (1) {
		// choose a topic
		printf("Enter topic: ");
		if (fgets(pi.topic, MAX_TOPIC_LEN, stdin) == NULL)
		{
			printf("fgets() failed: topic\n");
			return 1;
		}
		// trim newline character
		if (pi.topic[strlen(pi.topic) - 1] == '\n')
		{
			pi.topic[strlen(pi.topic) - 1] = '\0';
		}
		if (strcmp(pi.topic, "") == 0) {
			continue;
		}
		else if (strcmp(pi.topic, "exit") == 0)
		{
			shutdown(sock, SD_BOTH);
			closesocket(sock);
			WSACleanup();
			return 0;
		}
		else {
			break;
		}
	}

	// start publishing messages to chosen topic
	while (1) {
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