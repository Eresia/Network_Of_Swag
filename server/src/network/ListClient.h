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

	ListClient* createList();
	ItemList createElement(ClientNetwork*);
	void addClient(ListClient*, ClientNetwork*);
	void removeClient(ListClient*, ClientNetwork*);
	void removeClientByName(ListClient*, char*);
	ClientNetwork* getClientByName(ListClient*, char*);
	ClientNetwork* getClientByInfo(ListClient*, SOCKADDR_IN*);
	ClientNetwork* getLastClient(ListClient*);
	bool isInList(ListClient*, ClientNetwork*);
	bool isInListByName(ListClient*, char*);
	bool isInListByInfo(ListClient*, SOCKADDR_IN*);
	void closeAll(ListClient*);

	int addClient_Item(ItemList, ClientNetwork*);
	int removeClientByName_Item(ItemList, char*);
	ClientNetwork* getClientByName_Item(ItemList, char*);
	ClientNetwork* getClientByInfo_Item(ItemList, SOCKADDR_IN*);
	ClientNetwork* getLastClient_Item(ItemList);
	bool isInListByName_Item(ItemList, char*);
	bool isInListByInfo_Item(ItemList, SOCKADDR_IN*);
	void closeAll_Item(ItemList);

#endif
