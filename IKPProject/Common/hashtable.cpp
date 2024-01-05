#include "hashtable.h"

HASH_TABLE* init_hash_table()
{
	HASH_TABLE* table = (HASH_TABLE*)malloc(sizeof(HASH_TABLE));
	if (table == NULL)
	{
		printf("init_hash_table() failed: out of memory\n");
		return NULL;
	}

	table->items = (HASH_ITEM*)malloc(sizeof(HASH_ITEM) * TABLE_SIZE);
	if (table->items == NULL)
	{
		printf("init_hash_table() failed: out of memory\n");
		return NULL;
	}

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		table->items[i].list = NULL;
		table->items[i].key = (char*)malloc(sizeof(char) * MAX_KEY_LEN);
	}
	InitializeCriticalSection(&table->cs);

	return table;
}

int hash(const char* key)
{
	if (key == NULL)
	{
		printf("hash() failed: key is NULL\n");
		return -1;
	}

	int hash = 0;
	for (int i = 0; i < strlen(key); i++)
	{
		hash += key[i];
	}

	return hash % TABLE_SIZE;
}

bool add_list_table(HASH_TABLE* table, const char* key)
{
	if (table == NULL)
	{
		printf("add_list_table() failed: table is NULL\n");
		return false;
	}

	if (key == NULL)
	{
		printf("add_list_table() failed: key is NULL\n");
		return false;
	}

	if (strlen(key) > MAX_KEY_LEN)
	{
		printf("add_list_table() failed: key is too long\n");
		return false;
	}

	EnterCriticalSection(&table->cs);

	int index = hash(key);
	if (index == -1)
	{
		printf("add_list_table() failed: hash() failed\n");
		LeaveCriticalSection(&table->cs);
		return false;
	}

	table->items[index].list = init_list();
	if (table->items[index].list == NULL)
	{
		printf("add_list_table() failed: init_list() failed\n");
		LeaveCriticalSection(&table->cs);
		return false;
	}

	strcpy_s(table->items[index].key, MAX_KEY_LEN, key);

	LeaveCriticalSection(&table->cs);
	return true;
}

bool add_table_item(HASH_TABLE* table, const char* key, SOCKET sock)
{
	if (table == NULL)
	{
		printf("add_table_item() failed: table is NULL\n");
		return false;
	}

	if (key == NULL)
	{
		printf("add_table_item() failed: key is NULL\n");
		return false;
	}

	if (strlen(key) > MAX_KEY_LEN)
	{
		printf("add_table_item() failed: key is too long\n");
		return false;
	}


	EnterCriticalSection(&table->cs);

	int index = hash(key);
	if (index == -1)
	{
		printf("add_table_item() failed: hash() failed\n");
		LeaveCriticalSection(&table->cs);
		return false;
	}
	HASH_ITEM* item = &table->items[index];
	if (item->list == NULL) {
		printf("add_table_item() failed: list for key is NULL\n");
	}
	LIST_ITEM newItem = { sock, NULL };
	add_list_front(item->list, newItem);

	LeaveCriticalSection(&table->cs);
	return true;
}

LIST* get_table_item(HASH_TABLE* table, const char* key)
{
	if (table == NULL)
	{
		printf("get_table_item() failed: table is NULL\n");
		return NULL;
	}

	if (key == NULL)
	{
		printf("get_table_item() failed: key is NULL\n");
		return NULL;
	}

	if (strlen(key) > MAX_KEY_LEN)
	{
		printf("get_table_item() failed: key is too long\n");
		return NULL;
	}

	EnterCriticalSection(&table->cs);

	int index = hash(key);
	if (index == -1)
	{
		printf("get_table_item() failed: hash() failed\n");
		LeaveCriticalSection(&table->cs);
		return NULL;
	}

	HASH_ITEM item = table->items[index];
	LeaveCriticalSection(&table->cs);
	return item.list;
}

bool has_key(HASH_TABLE* table, const char* key)
{
	if (table == NULL)
	{
		printf("has_key() failed: table is NULL\n");
		return false;
	}

	if (key == NULL)
	{
		printf("has_key() failed: key is NULL\n");
		return false;
	}

	if (strlen(key) > MAX_KEY_LEN)
	{
		printf("has_key() failed: key is too long\n");
		return false;
	}

	EnterCriticalSection(&table->cs);

	int index = hash(key);
	if (index == -1)
	{
		printf("has_key() failed: hash() failed\n");
		LeaveCriticalSection(&table->cs);
		return false;
	}

	if (strcmp(table->items[index].key, key) != 0)
	{
		LeaveCriticalSection(&table->cs);
		return false;
	}

	LeaveCriticalSection(&table->cs);
	return true;
}

bool remove_table_item(HASH_TABLE* table, const char* key)
{
	if (table == NULL)
	{
		printf("remove_table_item() failed: table is NULL\n");
		return false;
	}

	if (key == NULL)
	{
		printf("remove_table_item() failed: key is NULL\n");
		return false;
	}

	if (strlen(key) > MAX_KEY_LEN)
	{
		printf("remove_table_item() failed: key is too long\n");
		return false;
	}

	EnterCriticalSection(&table->cs);

	int index = hash(key);
	if (index == -1)
	{
		printf("remove_table_item() failed: hash() failed\n");
		LeaveCriticalSection(&table->cs);
		return false;
	}

	if (strcmp(table->items[index].key, key) != 0)
	{
		printf("remove_table_item() failed: key not found\n");
		LeaveCriticalSection(&table->cs);
		return false;
	}

	if (!clear_list(table->items[index].list))
	{
		printf("remove_table_item() failed: clear_list() failed\n");
		LeaveCriticalSection(&table->cs);
		return false;
	}

	LeaveCriticalSection(&table->cs);
	return true;
}

bool free_hash_table(HASH_TABLE** table)
{
	if (table == NULL)
	{
		printf("free_hash_table() failed: table is NULL\n");
		return false;
	}

	if (*table == NULL)
	{
		printf("free_hash_table() failed: table is NULL\n");
		return false;
	}

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (!free_list(&(*table)->items[i].list))
		{
			printf("free_hash_table() failed: free_list() failed\n");
			return false;
		}
	}

	free((*table)->items);
	free(*table);
	*table = NULL;

	return true;
}

void print_hash_table(HASH_TABLE* table)
{
	if (table == NULL)
	{
		printf("print_hashtable() failed: table is NULL\n");
		return;
	}

	printf("======== Hash Table ========\n");
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (strcmp(table->items[i].key, "\0") != 0) {
			printf("Index: %d\n", i);
			printf("Key: %s\n", table->items[i].key);
			printf("List:\n");
			print_list(table->items[i].list);
			printf("\n");
		}
	}
	printf("============================\n");
}