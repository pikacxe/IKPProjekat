#include "pubsublib.h"

int main() {
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int numCPU = sysinfo.dwNumberOfProcessors;
	printf("Number of CPUs: %d\n", numCPU);
	HANDLE threadHandle;
	THR_ARGS args;

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
	HANDLE pubAcceptThreadHandle = CreateThread(NULL, 0, PUBAcceptThread, args.completionPort, 0, NULL);
	if (pubAcceptThreadHandle == NULL) {
		printf("Error creating publisher accept thread\n");
		WSACleanup();
		CloseHandle(args.completionPort);
		return 1;
	}

	// Create subscriber accept thread
	HANDLE subAcceptThreadHandle = CreateThread(NULL, 0, SUBAcceptThread, args.completionPort, 0, NULL);
	if (subAcceptThreadHandle == NULL) {
		printf("Error creating subscriber accept thread\n");
		WSACleanup();
		CloseHandle(args.completionPort);
		return 1;
	}


	printf("Press any key to exit...\n");
	getchar();

	CloseHandle(pubAcceptThreadHandle);
	CloseHandle(subAcceptThreadHandle);
	CloseHandle(args.completionPort);
	WSACleanup();
	return 0;
}