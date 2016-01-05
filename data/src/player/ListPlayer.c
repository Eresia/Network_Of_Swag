#include "ListPlayer.h"

ListPlayer* createListPlayer(){
	ListPlayer* list = malloc(sizeof(ListPlayer));
	list->nb = 0;
	list->firstItem = NULL;
	return list;
}

ItemListPlayer createElementPlayer(Player* player){
	ItemListPlayer elt = malloc(sizeof(struct ListPartPlayer));
	elt->player = player;
	elt->next = NULL;
	return elt;
}

int addPlayer(ListPlayer* list, Player* player){
	int result;
	if(list->firstItem == NULL){
		list->firstItem = createElementPlayer(player);
		list->nb++;
		#ifdef DEBUG
			printf("Client added\n");
		#endif
		result = NO_ERROR;
	}
	else if(list->firstItem->player == player){
		#ifdef DEBUG
			printf("Client already exist\n");
		#endif
		result = ELM_ALREADY_EXIST;
	}
	else{
		result = addPlayer_Item(list->firstItem, player);
		if(result == NO_ERROR){
			list->nb++;
			#ifdef DEBUG
				printf("Client added\n");
			#endif
		}
	}
	return result;
}

void removePlayer(ListPlayer* list, Player* player){
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

Player* getPlayerByName(ListPlayer* list, char* name){
	return getPlayerByName_Item(list->firstItem, name);
}

Player* getLastPlayer(ListPlayer* list){
	Player* result;
	result = getLastPlayer_Item(list->firstItem);
	return result;
}

bool isPlayerInList(ListPlayer* list, Player* player){
	bool result;
	result = isPlayerInListByName(list, player->name);
	return result;
}

bool isPlayerInListByName(ListPlayer* list, char* name){
	bool result;
	result = isPlayerInListByName_Item(list->firstItem, name);
	return result;
}

void saveAllPlayer(ListPlayer* list){
	saveAllPlayer_Item(list->firstItem);
}




int addPlayer_Item(ItemListPlayer item, Player* player){
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

Player* getPlayerByName_Item(ItemListPlayer item, char* name){
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

Player* getLastPlayer_Item(ItemListPlayer item){
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
