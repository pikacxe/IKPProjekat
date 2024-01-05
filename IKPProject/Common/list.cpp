#include "list.h"

LIST* init_list()
{
	LIST* list = (LIST*)malloc(sizeof(LIST));
	if (list == NULL)
	{
		printf("init_list() failed: out of memory\n");
		return NULL;
	}

	list->head = NULL;
	list->tail = NULL;
	list->count = 0;

	return list;
}

void add_list_front(LIST* list, LIST_ITEM data)
{
	if (list == NULL)
	{
		printf("add_list_front() failed: list is NULL\n");
		return;
	}

	LIST_ITEM* item = (LIST_ITEM*)malloc(sizeof(LIST_ITEM));
	if (item == NULL)
	{
		printf("add_list_front() failed: out of memory\n");
		return;
	}

	item->data = data.data;
	item->type = data.type;
	item->next = NULL;

	if (list->count == 0)
	{
		list->head = item;
		list->tail = item;
	}
	else 
	{
		item->next = list->head;
		list->head = item;
	}

	list->count++;
}


void add_list_back(LIST* list, LIST_ITEM data)
{
	if (list == NULL)
	{
		printf("add_list_back() failed: list is NULL\n");
		return;
	}

	LIST_ITEM* item = (LIST_ITEM*)malloc(sizeof(LIST_ITEM));
	if (item == NULL)
	{
		printf("add_list_back() failed: out of memory\n");
		return;
	}

	item->data = data.data;
	item->type = data.type;
	item->next = NULL;

	if (list->count == 0)
	{
		list->head = item;
		list->tail = item;
	}
	else
	{
		list->tail->next = item;
		list->tail = item;
	}

	list->count++;
}

LIST_ITEM* get_list_item(LIST* list, int index)
{
	if (list == NULL)
	{
		printf("get_list_item() failed: list is NULL\n");
		return NULL;
	}

	if (index < 0 || index >= list->count)
	{
		printf("get_list_item() failed: index out of range\n");
		return NULL;
	}

	LIST_ITEM* item = list->head;
	for (int i = 0; i < index; i++)
	{
		item = item->next;
	}

	return item;
}

bool remove_list(LIST* list, int index)
{
	if (list == NULL)
	{
		printf("remove_list() failed: list is NULL\n");
		return false;
	}

	if (index < 0 || index >= list->count)
	{
		printf("remove_list() failed: index out of range\n");
		return false;
	}

	LIST_ITEM* item = list->head;
	LIST_ITEM* prev = NULL;
	for (int i = 0; i < index; i++)
	{
		prev = item;
		item = item->next;
	}

	if (prev == NULL)
	{
		list->head = item->next;
	}
	else
	{
		prev->next = item->next;
	}

	if (item == list->tail)
	{
		list->tail = prev;
	}

	free(item);
	list->count--;

	return true;
}

bool clear_list(LIST* list)
{
	if (list == NULL)
	{
		printf("clear_list() failed: list is NULL\n");
		return false;
	}

	while (list->count > 0)
	{
		if (!remove_list(list, 0))
		{
			printf("[WARN] clear_list() failed: failed to remove element from the list\n");
		}
	}

	return true;
}

bool free_list(LIST** list)
{
	if (list == NULL)
	{
		printf("free_list() failed: list is NULL\n");
		return false;
	}

	if (*list == NULL)
	{
		printf("free_list() failed: list is NULL\n");
		return false;
	}

	if (!clear_list(*list))
	{
		printf("free_list() failed: clear_list() failed\n");
		return false;
	}

	free(*list);
	*list = NULL;

	return true;
}

void print_list_item(LIST_ITEM* data)
{
	if (data == NULL)
	{
		printf("print_list_item() failed: data is NULL\n");
		return;
	}
	// print element based on type
	switch (data->type)
	{
	case text:
		printf("%s, \n", (char*)data->data);
		break;
	case number:
		printf("%d, \n", *(int*)data->data);
		break;
	case publisher:
		printf("Not implemented\n");
		//printf("%s\n", ((PUBLISHER*)data->data)->name);
		break;
	case subscriber:
		printf("Not implemented\n");
		//printf("%s\n", ((SUBSCRIBER*)data->data)->name);
		break;

	}
}

void print_list(LIST* list)
{
	if (list == NULL)
	{
		printf("print_list() failed: list is NULL\n");
		return;
	}

	if (list->count == 0)
	{
		printf("print_list(): list is empty\n");
		return;
	}
	LIST_ITEM* item = list->head;
	printf("-------- List --------\n");
	printf("List count: %d\n[", list->count);
	while (item != NULL)
	{
		print_list_item(item);
		item = item->next;
	}
	printf("]\n");
	printf("----------------------\n");
}