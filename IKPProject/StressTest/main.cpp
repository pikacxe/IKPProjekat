#include <stdio.h>
#include <stdlib.h>
#include "../Common/common.h"


void stress_tests();
void start_server();
void start_subscriber(int min);
void start_publisher(int min);
int pub_test(char topic[], char msg[], int num);

int main() {

	printf("Prepare stress tests...\n\n");

	stress_tests();

	return 0;
}


void stress_tests() {
	start_server();
	// menu
	while (1) {
		char option;
		printf("Choose an option:\n");
		printf("1. 1 publisher and messages\n");
		printf("2. Connect publishers and subscribers\n");
		printf("s. Start subscriber\n");
		printf("p. Start publisher\n");
		printf("q. Quit\n");
		printf("Enter option: ");
		option = getchar();
		fflush(stdin);
		if (option == 'q')
		{
			break;
		}
		else if (option == '1')
		{
			char topic[] = "topic1";
			char msg[] = "message1";
			printf("Number of messages:\n");
			int num;
			scanf_s("%d", &num);
			fflush(stdin);

			pub_test(topic, msg, num);


		}
		else if (option == '2') {
			int pubs;
			int subs;
			printf("Enter number of publishers:");
			scanf_s("%d", &pubs);
			fflush(stdin);
			printf("Enter number of subscribers:");
			scanf_s("%d", &subs);
			fflush(stdin);
			for (int i = 0; i < pubs; i++) {
				printf("[%d] ", i + 1);
				start_publisher(TRUE);
			}
			printf("\n\n");
			for (int i = 0; i < subs; i++) {
				printf("[%d] ", i + 1);
				start_subscriber(TRUE);
			}
		}
		else if (option == 's') {
			start_subscriber(FALSE);
		}
		else if (option == 'p') {
			start_publisher(FALSE);
		}
		else {
			printf("Invalid option\n\n");
		}
	}
}

int pub_test(char topic[], char msg[], int num) {
	printf("Preparing publisher...\n\n");

	SOCKET sock = connect(PUB_PORT);
	if (sock == INVALID_SOCKET)
	{
		printf("Failed to connect to server\n");
		return 1;
	}
	PUB_INFO pi;
	memset(&pi, 0, sizeof(PUB_INFO));
	strcpy_s(pi.topic, topic);
	strcpy_s(pi.msg, msg);
	// start publishing messages to chosen topic
	for (int i = 0; i < num; i++) {
		printf("Publishing message %d for \"%s\": %s\n", i + 1, pi.topic, pi.msg);
		if (send(sock, (char*)&pi, sizeof(PUB_INFO), 0) == SOCKET_ERROR)
		{
			printf("send() failed: %d\n", WSAGetLastError());
			break;
		}
		Sleep(100);
	}
	strcpy_s(pi.msg, strlen("exit") + 1, "exit");
	send(sock, (char*)&pi, sizeof(PUB_INFO), 0);
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	WSACleanup();

	return 0;
}

void start_server() {
	const char* server_exe = "cd C:\\Users\\lix20\\Desktop\\IKP Projekat\\IKPProject\\x64\\Debug\\ && start /i PubSubEngine.exe";
	if (system(server_exe) == -1)
	{
		printf("Failed to launch server\n");
		return;
	}
	printf("Server started\n");
}

void start_publisher(int min) {
	if (min == TRUE) {
		const char* publisher_exe = "cd C:\\Users\\lix20\\Desktop\\IKP Projekat\\IKPProject\\x64\\Debug\\ && start /i /min Publisher.exe";
		if (system(publisher_exe) == -1)
		{
			printf("Failed to launch publisher\n");
			return;
		}
		printf("Publisher started\n");
	}
	else {
		const char* publisher_exe = "cd C:\\Users\\lix20\\Desktop\\IKP Projekat\\IKPProject\\x64\\Debug\\ && start /i Publisher.exe";
		if (system(publisher_exe) == -1)
		{
			printf("Failed to launch publisher\n");
			return;
		}
		printf("Publisher started\n");
	}
}


void start_subscriber(int min) {
	if (min == TRUE) {
		const char* subscriber_exe = "cd C:\\Users\\lix20\\Desktop\\IKP Projekat\\IKPProject\\x64\\Debug\\ && start /i /min Subscriber.exe";
		if (system(subscriber_exe) == -1)
		{
			printf("Failed to launch subscriber\n");
			return;
		}
		printf("Subscriber started\n");
	}
	else {
		const char* subscriber_exe = "cd C:\\Users\\lix20\\Desktop\\IKP Projekat\\IKPProject\\x64\\Debug\\ && start /i Subscriber.exe";
		if (system(subscriber_exe) == -1)
		{
			printf("Failed to launch subscriber\n");
			return;
		}
		printf("Subscriber started\n");
	}
}