#ifndef LIST_CLIENT
	#define LIST_CLIENT

	#include <stdlib.h>
	#include <stdio.h>

	#include "../../../data/src/usefull/Usefull.h"
	#include "ClientNetwork.h"

	#define ELM_ALREADY_EXIST 1
	#define ELM_NOT_EXIST 2
	#define OTHER 3

	typedef struct ListPart* ItemList;
	struct ListPart{
		ClientNetwork* client;
		struct ListPart* next;
	};

	typedef struct ListClient ListClient;
	struct ListClient{
		int nb;
		ItemList firstItem;
	};

	ListClient createList();
	void addClient(ListClient, ClientNetwork*);
	void removeClient(ListClient, ClientNetwork*);
	void removeClientById(ListClient, int);
	ClientNetwork* getClientById(ListClient, int);
	ClientNetwork* getLastClient(ListClient);
	bool isInList(ListClient, ClientNetwork*);

	int addClient_Item(ItemList, ClientNetwork*);
	int removeClientById_Item(ItemList, int);
	ClientNetwork* getClientById_Item(ItemList, int);
	ClientNetwork* getLastClient_Item(ItemList);
	bool isInList_Item(ItemList, ClientNetwork*);

#endif
