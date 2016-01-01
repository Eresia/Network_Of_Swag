#include "ListClient.h"

ListClient* createList(){
	ListClient* list = malloc(sizeof(ListClient));
	list->nb = 0;
	list->firstItem = NULL;
	return list;
}

ItemList createElement(ClientNetwork* client){
	ItemList elt = malloc(sizeof(struct ListPart));
	elt->client = client;
	elt->next = NULL;
	return elt;
}

void addClient(ListClient* list, ClientNetwork* client){
	if(list->firstItem == NULL){
		list->firstItem = createElement(client);
		list->nb++;
		#ifdef DEBUG
			printf("Client added\n");
		#endif

	}
	else if(list->firstItem->client == client){
		#ifdef DEBUG
			printf("Client already exist\n");
		#endif
	}
	else{
		if(addClient_Item(list->firstItem, client) == NO_ERROR){
			list->nb++;
			#ifdef DEBUG
				printf("Client added\n");
			#endif
		}
	}
}

void removeClient(ListClient* list, ClientNetwork* client){
	return removeClientByName(list, client->name);
}

void removeClientByName(ListClient* list, char* name){
	if(list->firstItem == NULL){
		#ifdef DEBUG
			printf("Remove - No client\n");
		#endif
	}
	else if(strcmp(list->firstItem->client->name,name) == 0){
		ItemList next = list->firstItem->next;
		//closesocket(list->firstItem->client->socket_tcp);
		//closesocket(list->firstItem->client->socket_udp);
		free(list->firstItem);
		list->firstItem = next;
		list->nb--;
	}
	else{
		if(removeClientByName_Item(list->firstItem, name) == NO_ERROR){
			list->nb--;
		}
	}
}

ClientNetwork* getClientByName(ListClient* list, char* name){
	return getClientByName_Item(list->firstItem, name);
}

ClientNetwork* getClientByInfo(ListClient* list, SOCKADDR_IN* info){
	return getClientByInfo_Item(list->firstItem, info);
}

ClientNetwork* getLastClient(ListClient* list){
	return getLastClient_Item(list->firstItem);
}

bool isInList(ListClient* list, ClientNetwork* client){
	return isInListByInfo(list, client->info);
}

bool isInListByName(ListClient* list, char* name){
	return isInListByName_Item(list->firstItem, name);
}

bool isInListByInfo(ListClient* list, SOCKADDR_IN* info){
	return isInListByInfo_Item(list->firstItem, info);
}

void closeAll(ListClient* list){
	closeAll_Item(list->firstItem);
}




int addClient_Item(ItemList item, ClientNetwork* client){
	if(item == NULL){
		return OTHER;
	}
	else if(item->next != NULL){
		if(strcmp(item->next->client->name,client->name) > 0){
			return ELM_ALREADY_EXIST;
		}
		else{
			return addClient_Item(item->next, client);
		}
	}
	else{
		item->next = createElement(client);
		return NO_ERROR;
	}
}

int removeClientByName_Item(ItemList item, char* name){
	if((item == NULL) || (item->next == NULL)){
		return ELM_NOT_EXIST;
	}
	else if(strcmp(item->next->client->name, name) == 0){
		ItemList next = item->next->next;
		//closesocket(item->client->socket_tcp);
		//closesocket(item->client->socket_udp);
		free(item->next);
		item->next = next;
		return NO_ERROR;
	}
	else{
		return removeClientByName_Item(item->next, name);
	}
}

ClientNetwork* getClientByName_Item(ItemList item, char* name){
	if(item == NULL){
		return NULL;
	}
	else if(strcmp(item->client->name, name) == 0){
		return item->client;
	}
	else{
		return getClientByName_Item(item->next, name);
	}
}

ClientNetwork* getClientByInfo_Item(ItemList item, SOCKADDR_IN* info){
	if(item == NULL){
		return NULL;
	}
	else if((item->client->info->sin_addr.s_addr == info->sin_addr.s_addr) && (item->client->info->sin_port == info->sin_port)){
		return item->client;
	}
	else{
		return getClientByInfo_Item(item->next, info);
	}
}

ClientNetwork* getLastClient_Item(ItemList item){
	if(item == NULL){
		return NULL;
	}
	else if(item->next == NULL){
		return item->client;
	}
	else{
		return getLastClient_Item(item->next);
	}
}

bool isInListByName_Item(ItemList item, char* name){
	if(item == NULL){
		return false;
	}
	else if(strcmp(item->client->name, name) == 0){
		return true;
	}
	else{
		return isInListByName_Item(item->next, name);
	}
}

bool isInListByInfo_Item(ItemList item, SOCKADDR_IN* info){
	if(item == NULL){
		return false;
	}
	else if((item->client->info->sin_addr.s_addr == info->sin_addr.s_addr) && (item->client->info->sin_port == info->sin_port)){
		return true;
	}
	else{
		return isInListByInfo_Item(item->next, info);
	}
}

void closeAll_Item(ItemList item){
	if(item == NULL){
		#ifdef DEBUG
			printf("List free\n");
		#endif
	}
	else{
		ItemList next = item->next;
		//closesocket(item->client->socket_tcp);
		//closesocket(item->client->socket);
		free(item->client);
		free(item);
		closeAll_Item(next);
	}
}
