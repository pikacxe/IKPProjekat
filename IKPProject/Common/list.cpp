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
	InitializeCriticalSection(&list->cs);

	return list;
}

void add_list_front(LIST* list, LIST_ITEM data)
{
	if (list == NULL)
	{
		printf("add_list_front() failed: list is NULL\n");
		return;
	}
	EnterCriticalSection(&list->cs);
	LIST_ITEM* item = (LIST_ITEM*)malloc(sizeof(LIST_ITEM));
	if (item == NULL)
	{
		printf("add_list_front() failed: out of memory\n");
		LeaveCriticalSection(&list->cs);
		return;
	}

	item->data = data.data;
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
	LeaveCriticalSection(&list->cs);
}


void add_list_back(LIST* list, LIST_ITEM data)
{
	if (list == NULL)
	{
		printf("add_list_back() failed: list is NULL\n");
		return;
	}

	EnterCriticalSection(&list->cs);
	LIST_ITEM* item = (LIST_ITEM*)malloc(sizeof(LIST_ITEM));
	if (item == NULL)
	{
		printf("add_list_back() failed: out of memory\n");
		LeaveCriticalSection(&list->cs);
		return;
	}

	item->data = data.data;
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
	LeaveCriticalSection(&list->cs);
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

bool remove_from_list(LIST* list, int index)
{
	if (list == NULL)
	{
		printf("remove_list(): list is NULL\n");
		return true;
	}

	if (index < 0 || index >= list->count)
	{
		printf("remove_list() failed: index out of range\n");
		return false;
	}

	EnterCriticalSection(&list->cs);
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
	closesocket(item->data);
	free(item);
	item = NULL;
	list->count--;
	LeaveCriticalSection(&list->cs);

	return true;
}

bool clear_list(LIST* list)
{
	if (list != NULL)
	{
		EnterCriticalSection(&list->cs);
		while (list->count > 0)
		{
			if (!remove_from_list(list, 0))
			{
				printf("[WARN] clear_list() failed: failed to remove element from the list\n");
			}
		}
		LeaveCriticalSection(&list->cs);
	}

	return true;
}

bool free_list(LIST** list)
{
	if (list == NULL)
	{
		return true;
	}

	if (*list == NULL)
	{
		return true;
	}
	if ((*list)->count != 0)
	{
		EnterCriticalSection(&(*list)->cs);
		while ((*list)->head != (*list)->tail)
		{
			LIST_ITEM* item = (*list)->head;
			(*list)->head = item->next;
			shutdown(item->data, SD_BOTH);
			closesocket(item->data);
			free(item);
			(*list)->count--;
		}
		free((*list)->head);
		(*list)->head = NULL;
		(*list)->tail = NULL;
		(*list)->count = 0;
		LeaveCriticalSection(&(*list)->cs);
	}
	DeleteCriticalSection(&(*list)->cs);
	free(*list);
	return true;
}


void print_list(LIST* list)
{
	if (list == NULL)
	{
		printf("print_list() failed: list is NULL\n");
		return;
	}

	if (list->count <= 0)
	{
		printf("print_list(): list is empty\n");
		return;
	}
	LIST_ITEM* item = list->head;
	printf("-------- List --------\n");
	printf("List count: %d\n[", list->count);
	while (item != NULL)
	{
		printf("Socket: %d, ", item->data);
		item = item->next;
	}
	printf("]\n");
	printf("----------------------\n");
}