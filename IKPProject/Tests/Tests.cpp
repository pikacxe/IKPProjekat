#include <iostream>
#include "../Common/list.h"
#include "../Common/hashtable.h"

int main()
{
	printf("Preparing list tests...\n\n");

	int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	#pragma region ListTests


	LIST* list = init_list();
	// add 10 items to list
	for (int i = 0; i < 10; i++)
	{
		LIST_ITEM item;
		item.data = &data[i];
		item.type = number;
		add_list_back(list, item);
	}
	for (int i = 0; i < 10; i++)
	{
		LIST_ITEM item;
		item.data = &data[i];
		item.type = number;
		add_list_front(list, item);
	}
	// print list
	print_list(list);
	// get list item at index 5
	LIST_ITEM* item = get_list_item(list, 5);
	printf("Item at index 5: ");
	if (item != NULL)
	{
		print_list_item(item);
	}
	else
	{
		printf("Not found\n");
	}
	// remove list item at index 5
	printf("Removing item at index 5...\n");
	if (remove_from_list(list, 5))
	{
		printf("Item removed\n");
	}
	else
	{
		printf("Item not removed\n");
	}
	// print list
	print_list(list);
	// free list
	if (free_list(&list)) {
		printf("List destroyed\n");
	}
	else
	{
		printf("List not destroyed\n");
	}
	print_list(list);
	#pragma endregion

	// clear console
	//system("cls");
	printf("Preparing hash table tests...\n\n");
	#pragma region HashTableTests
	HASH_TABLE* table = init_hash_table();
	// add 10 items to table
	for (int i = 0; i < 10; i++)
	{
		LIST_ITEM item;
		item.data = &data[i];
		item.type = number;
		add_table_item(table, "123" + i, item);
	}
	// print table
	print_hash_table(table);
	// get table item with key "5"
	LIST* table_item = get_table_item(table, "5");
	printf("Item with key \"5\": ");
	if (table_item != NULL)
	{
		print_list(table_item);
	}
	else
	{
		printf("Not found\n");
	}
	// remove table item with key "5"
	printf("Removing item with key \"5\"...\n");
	if (remove_table_item(table, "5"))
	{
		printf("Item removed\n");
	}
	else
	{
		printf("Item not removed\n");
	}
	// print table
	print_hash_table(table);
	// free table
	if (free_hash_table(&table))
	{
		printf("Table destroyed\n");
	}
	else
	{
		printf("Table not destroyed\n");
	}
	print_hash_table(table);
	#pragma endregion
	printf("Press any key to exit...\n");
	getchar();
}
