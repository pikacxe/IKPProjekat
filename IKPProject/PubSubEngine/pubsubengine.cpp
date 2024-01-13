#include "pubsublib.h"

void menu(HASH_TABLE* table);

int main() {
	printf("Preparing PubSubEngine...\n\n");
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int numCPU = sysinfo.dwNumberOfProcessors;
	printf("Number of CPUs: %d\n", numCPU);
	HANDLE threadHandle;
	THR_ARGS args;
	cancelationToken = FALSE;
	args.hashTable = init_hash_table();

	if (args.hashTable == NULL) {
		printf("Error creating hash table\n");
		return 1;
	}

	if (InitializeWinsock() == 0) {
		printf("Error initializing winsock\n");
		return 1;
	}
	args.completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (args.completionPort == NULL) {
		printf("Error creating completion port\n");
		return 1;
	}
	for (int i = 0; i < numCPU; i++) {
		threadHandle = CreateThread(NULL, 0, WorkerThread, (LPVOID)&args, 0, NULL);
		if (threadHandle == NULL) {
			printf("Error creating thread\n");
			WSACleanup();
			CloseHandle(args.completionPort);
			return 1;
		}
		CloseHandle(threadHandle);
	}
	// Create publisher accept thread
	HANDLE pubAcceptThreadHandle = CreateThread(NULL, 0, PUBAcceptThread, (LPVOID)&args, 0, NULL);
	if (pubAcceptThreadHandle == NULL) {
		printf("Error creating publisher accept thread\n");
		WSACleanup();
		CloseHandle(args.completionPort);
		return 1;
	}

	// Create subscriber accept thread
	HANDLE subAcceptThreadHandle = CreateThread(NULL, 0, SUBAcceptThread, (LPVOID)&args, 0, NULL);
	if (subAcceptThreadHandle == NULL) {
		printf("Error creating subscriber accept thread\n");
		WSACleanup();
		CloseHandle(args.completionPort);
		return 1;
	}

	// Wait for user to exit
	menu(args.hashTable);

	cancelationToken = TRUE;

	// Cleanup
	CloseHandle(pubAcceptThreadHandle);
	CloseHandle(subAcceptThreadHandle);
	CloseHandle(args.completionPort);

	// free unused memory
	free_hash_table(&args.hashTable);
	free(args.hashTable);
	WSACleanup();
	printf("Wait for threads to finish (5s)\n");
	Sleep(5000);
	printf("Press any key to exit...\n");
	getchar();
	return 0;
}


void menu(HASH_TABLE* table) {
	while (1) {
		fflush(stdin);
		char option;
		printf("Options:\n");
		printf("1. Print hashtable\n");
		printf("q. Quit\n");

		printf("Enter option: ");
		option = getchar();
		fflush(stdin);

		if (option == '1') {
			print_hash_table(table);
		}
		else if (option == 'q') {
			fflush(stdin);
			break;
		}
		else {
			printf("Invalid option\n");
		}
	}
}