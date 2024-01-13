#include <stdio.h>
#include "../Common/common.h"
#include "../Common/hashtable.h"


void test_list(int num);
void test_hash_table(int keys, int max_socks);
void test_get_ht_keys();


int main() {


	printf("Begin testing...\n\n");

	printf("List testing...\n");

	test_list(10);

	printf("End list testing...\n");

	printf("Hash table testing...\n");

	test_hash_table(3, 5);

	printf("\n");

	test_get_ht_keys();

	printf("End hash table testing...\n");

	printf("End testing...\n");

	return 0;
}


void test_list(int num) {
	LIST* list = init_list();
	if (list == NULL) {
		printf("init_list() failed\n");
		return;
	}

	LIST_ITEM item;
	for (int i = 0; i < num; i++) {
		item.data = i;
		add_list_front(list, item);
	}

	print_list(list);

	for (int i = 0; i < num; i++) {
		item.data = i;
		add_list_back(list, item);
	}

	print_list(list);

	for (int i = 0; i < num / 2; i++) {
		remove_from_list(list, rand() % (list->count - 1) + 1);
	}

	print_list(list);

	clear_list(list);

	print_list(list);

	free_list(&list);

	print_list(list);
	// passed with no leaks
}

void test_hash_table(int keys, int max_socks) {
	HASH_TABLE* table = init_hash_table();
	if (table == NULL) {
		printf("init_hash_table() failed\n");
		return;
	}

	char* key = (char*)malloc(sizeof(char) * MAX_KEY_LEN);
	if (key == NULL) {
		printf("malloc() failed\n");
		return;
	}
	// add keys and random sockets per key
	for (int i = 0; i < keys; i++) {
		sprintf_s(key, MAX_KEY_LEN, "key%d", i);
		add_list_table(table, key);
		for (int j = 0; j < rand() % max_socks; j++) {
			add_table_item(table, key, rand() % 100);
		}
	}

	print_hash_table(table);

	// remove random keys
	for (int i = 0; i < keys / 2; i++) {
		sprintf_s(key, MAX_KEY_LEN, "key%d", rand() % keys);
		remove_table_item(table, key);
	}

	print_hash_table(table);

	free_hash_table(&table);
	free(key);
	// passed with no leaks
}

void test_get_ht_keys() {
	HASH_TABLE* table = init_hash_table();
	if (table == NULL) {
		printf("init_hash_table() failed\n");
		return;
	}

	char* key = (char*)malloc(sizeof(char) * MAX_KEY_LEN);
	if (key == NULL) {
		printf("malloc() failed\n");
		return;
	}
	// add keys and random sockets per key
	for (int i = 0; i < 5; i++) {
		sprintf_s(key, MAX_KEY_LEN, "key%d", i);
		add_list_table(table, key);
		for (int j = 0; j < rand() % 5; j++) {
			add_table_item(table, key, rand() % 100);
		}
	}

	print_hash_table(table);

	TOPIC_INFO keys;
	for (int i = 0; i < 3; i++) {
		get_table_keys(table, &keys);
		if (keys.count == 0) {
			printf("get_ht_keys() failed\n");
			return;
		}

		for (int i = 0; i < keys.count; i++) {
			printf("%s\n", keys.topics[i]);
		}

		for (int i = 0; i < keys.count; i++) {
			free(keys.topics[i]);
		}

		free(keys.topics);
		printf("\n\n");
	}
	free_hash_table(&table);
	free(key);
	// passed with no leaks
}