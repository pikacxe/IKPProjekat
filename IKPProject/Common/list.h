#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>



#define MAX_LIST_SIZE 100

typedef struct _LIST_ITEM
{
	SOCKET data;
	struct _LIST_ITEM* next;
} LIST_ITEM;

typedef struct _LIST
{
	LIST_ITEM* head;
	LIST_ITEM* tail;
	int count;
} LIST;

/// <summary>
/// Initialize list
/// </summary>
/// <returns>Pointer to initialized list</returns>
LIST* init_list();

/// <summary>
/// Add item to front of the list
/// </summary>
/// <param name="list"> - source list</param>
/// <param name="data"> - list item to be added</param>
void add_list_front(LIST* list, LIST_ITEM data);

/// <summary>
/// Add item to back of the list
/// </summary>
/// <param name="list"> - source list</param>
/// <param name="data"> - list item to be added</param>
void add_list_back(LIST* list, LIST_ITEM data);

/// <summary>
/// Get list item at index
/// </summary>
/// <param name="list"> - source list</param>
/// <param name="index"> - index of the list item</param>
/// <returns>Returns list item at given index if exists, otherwise NULL </returns>
LIST_ITEM* get_list_item(LIST* list, int index);

/// <summary>
/// Remove list item at index
/// </summary>
/// <param name="list"> - source list</param>
/// <param name="index"> - index of the list item</param>
/// <returns>True if successful, otherwise false</returns>
bool remove_from_list(LIST* list, int index);

/// <summary>
/// Clear list
/// </summary>
/// <param name="list"></param>
/// <returns>True if successful, otherwise false</returns>
bool clear_list(LIST* list);

/// <summary>
/// Destroy list
/// </summary>
/// <param name="list"></param>
/// <returns>True if successful, otherwise false</returns>
bool free_list(LIST** list);

/// <summary>
/// Print list
/// </summary>
/// <param name="list"> - source list</param>
void print_list(LIST* list);

