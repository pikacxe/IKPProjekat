#pragma once
#include "string.h"
#include "list.h"

#define TABLE_SIZE 1000
#define MAX_KEY_LEN 128 + 1

typedef struct _HASH_ITEM
{
	char* key;
	LIST* list;
} HASH_ITEM;

typedef struct _HASH_TABLE
{
	CRITICAL_SECTION cs;
	HASH_ITEM* items;
} HASH_TABLE;

/// <summary>
/// Initialize hash table
/// </summary>
/// <returns>Handle if initialization is successful, otherwise NULL</returns>
HASH_TABLE* init_hash_table();

/// <summary>
/// Hash function
/// </summary>
/// <param name="key"> - key to hash</param>
/// <returns>Hashed key</returns>
int hash(const char* key);

/// <summary>
/// Add item to hash table
/// </summary>
/// <param name="table"> - source table</param>
/// <param name="key"> - table item key</param>
/// <param name="item"> - item to add</param>
/// <returns>True if addition is successful, otherwise false</returns>
bool add_table_item(HASH_TABLE* table, const char* key, SOCKET item);

/// <summary>
/// Add list to hash table
/// </summary>
/// <param name="table"> - source table</param>
/// <param name="key"> - table item key</param>
/// <returns>True if addition is successful, otherwise false</returns>
bool add_list_table(HASH_TABLE* table, const char* key);

/// <summary>
/// Get table item
/// </summary>
/// <param name="table"> - source table</param>
/// <param name="key"> - item key</param>
/// <returns>Table item if exists, otherwise NULL</returns>
LIST* get_table_item(HASH_TABLE* table, const char* key);

/// <summary>
/// Check if table has item with specified key
/// </summary>
/// <param name="table"> - source table</param>
/// <param name="key"> - item key</param>
/// <returns>True if table has item with specified key, otherwise false</returns>
bool has_key(HASH_TABLE* table, const char* key);

/// <summary>
/// Remove item from table
/// </summary>
/// <param name="table"> - source table</param>
/// <param name="key"> - item key</param>
/// <returns>True if table item successfully removed, otherwise false</returns>
bool remove_table_item(HASH_TABLE* table, const char* key);

/// <summary>
/// Free hash table
/// </summary>
/// <param name="table"> - table to free</param>
/// <returns>True if table successfully freed, otherwise false</returns>
bool free_hash_table(HASH_TABLE** table);

/// <summary>
/// Print hash table
/// </summary>
/// <param name="table"> - table to print</param>
void print_hash_table(HASH_TABLE* table);
