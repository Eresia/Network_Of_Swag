#include "ListClient.h"

ListClient createList(){
	ListClient list;
	list.nb = 0;
	list.firstItem = NULL;
	return list;
}

ItemList createElement(ClientNetwork* client){
	ItemList Elt = malloc(sizeof(struct ListPart));
	Elt->client = client;
	Elt->next = NULL;
	return Elt;
}

void addClient(ListClient list, ClientNetwork* client){
	if(list.firstItem == NULL){
		list.firstItem = createElement(client);
		list.nb++;
	}
	else if(list.firstItem->client != client){
		#ifdef DEBUG
			printf("Client already exist\n");
		#endif
	}
	else{
		if(addClient_Item(list.firstItem, client) == NO_ERROR){
			list.nb++;
		}
	}
}

void removeClient(ListClient list, ClientNetwork* client){
	return removeClientById(list, client->id);
}

void removeClientById(ListClient list, int id){
	if(list.firstItem == NULL){
		#ifdef DEBUG
		printf("Remove - No client\n")
		#endif
	}
	else if(list.firstItem->client->id == id){
		free(list.firstItem->client);
		free(list.firstItem);
		list.firstItem = NULL;
		list.nb--;
	}
	else{
		if(removeClientById_Item(list.firstItem, id) == NO_ERROR){
			list.nb--;
		}
	}
}

ClientNetwork* getClientById(ListClient list, int id){
	return getClientById_Item(list.firstItem, id);
}

ClientNetwork* getLastClient(ListClient list){
	return getLastClient_Item(list.firstItem);
}

bool isInList(ListClient list, ClientNetwork* client){
	return isInList_Item(list.firstItem, client);
}




int addClient_Item(ItemList item, ClientNetwork* client){
	if(item == NULL){
		return OTHER;
	}
	else if(item->next != NULL){
		if(item->next->client == client){
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

int removeClientById_Item(ItemList item, int id){
	if((item == NULL) || (item->next == NULL)){
		return ELM_NOT_EXIST;
	}
	else if(item->next->client->id == id){
		ItemList next = item->next->next;
		free(item->next->client);
		free(item->next);
		item->next = next;
		return NO_ERROR;
	}
	else{
		return removeClientById_Item(item->next, id);
	}
}

ClientNetwork* getClientById_Item(ItemList item, int id){
	if(item == NULL){
		return NULL;
	}
	else if(item->client->id == id){
		return item->client;
	}
	else{
		return getClientById_Item(item->next, id);
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

bool isInList_Item(ItemList item, ClientNetwork* client){
	if(item == NULL){
		return false;
	}
	else if(item->client == client){
		return true;
	}
	else{
		return isInList_Item(item->next, client);
	}
}
