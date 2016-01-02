#include "ListPlayer.h"

ListPlayer* createListPlayer(){
	ListPlayer* list = malloc(sizeof(ListPlayer));
	list->nb = 0;
	list->firstItem = NULL;
	return list;
}

ItemListPlayer createElementPlayer(player* player){
	ItemListPlayer elt = malloc(sizeof(struct ListPartPlayer));
	elt->player = player;
	elt->next = NULL;
	return elt;
}

void addPlayer(ListPlayer* list, player* player){
	if(list->firstItem == NULL){
		list->firstItem = createElementPlayer(player);
		list->nb++;
		#ifdef DEBUG
			printf("Client added\n");
		#endif

	}
	else if(list->firstItem->player == player){
		#ifdef DEBUG
			printf("Client already exist\n");
		#endif
	}
	else{
		if(addPlayer_Item(list->firstItem, player) == NO_ERROR){
			list->nb++;
			#ifdef DEBUG
				printf("Client added\n");
			#endif
		}
	}
}

void removePlayer(ListPlayer* list, player* player){
	return removePlayerByName(list, player->name);
}

void removePlayerByName(ListPlayer* list, char* name){
	if(list->firstItem == NULL){
		#ifdef DEBUG
			printf("Remove - No client\n");
		#endif
	}
	else if(strcmp(list->firstItem->player->name,name) == 0){
		ItemListPlayer next = list->firstItem->next;
		//closesocket(list->firstItem->client->socket_tcp);
		//closesocket(list->firstItem->client->socket_udp);
		free(list->firstItem);
		list->firstItem = next;
		list->nb--;
	}
	else{
		if(removePlayerByName_Item(list->firstItem, name) == NO_ERROR){
			list->nb--;
		}
	}
}

player* getPlayerByName(ListPlayer* list, char* name){
	return getPlayerByName_Item(list->firstItem, name);
}

player* getLastPlayer(ListPlayer* list){
	return getLastPlayer_Item(list->firstItem);
}

bool isPlayerInList(ListPlayer* list, player* player){
	return isPlayerInListByName(list, player->name);
}

bool isPlayerInListByName(ListPlayer* list, char* name){
	return isPlayerInListByName_Item(list->firstItem, name);
}

void closeAllPlayer(ListPlayer* list){
	closeAllPlayer_Item(list->firstItem);
}




int addPlayer_Item(ItemListPlayer item, player* player){
	if(item == NULL){
		return OTHER;
	}
	else if(item->next != NULL){
		if(strcmp(item->next->player->name,player->name) == 0){
			return ELM_ALREADY_EXIST;
		}
		else{
			return addPlayer_Item(item->next, player);
		}
	}
	else{
		item->next = createElementPlayer(player);
		return NO_ERROR;
	}
}

int removePlayerByName_Item(ItemListPlayer item, char* name){
	if((item == NULL) || (item->next == NULL)){
		return ELM_NOT_EXIST;
	}
	else if(strcmp(item->next->player->name, name) == 0){
		ItemListPlayer next = item->next->next;
		//closesocket(item->client->socket_tcp);
		//closesocket(item->client->socket_udp);
		free(item->next);
		item->next = next;
		return NO_ERROR;
	}
	else{
		return removePlayerByName_Item(item->next, name);
	}
}

player* getPlayerByName_Item(ItemListPlayer item, char* name){
	if(item == NULL){
		return NULL;
	}
	else if(strcmp(item->player->name, name) == 0){
		return item->player;
	}
	else{
		return getPlayerByName_Item(item->next, name);
	}
}

player* getLastPlayer_Item(ItemListPlayer item){
	if(item == NULL){
		return NULL;
	}
	else if(item->next == NULL){
		return item->player;
	}
	else{
		return getLastPlayer_Item(item->next);
	}
}

bool isPlayerInListByName_Item(ItemListPlayer item, char* name){
	if(item == NULL){
		return false;
	}
	else if(strcmp(item->player->name, name) == 0){
		return true;
	}
	else{
		return isPlayerInListByName_Item(item->next, name);
	}
}

void closeAllPlayer_Item(ItemListPlayer item){
	if(item == NULL){
		#ifdef DEBUG
			printf("List free\n");
		#endif
	}
	else{
		ItemListPlayer next = item->next;
		//closesocket(item->client->socket_tcp);
		//closesocket(item->client->socket);
		free(item->player);
		free(item);
		closeAllPlayer_Item(next);
	}
}
