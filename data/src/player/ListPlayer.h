#ifndef LIST_PLAYER
	#define LIST_PLAYER

	#include <stdlib.h>
	#include <stdio.h>
	#include <pthread.h>

	#include "../../../data/src/usefull/Usefull.h"
	#include "Player.h"

	#define ELM_ALREADY_EXIST 1
	#define ELM_NOT_EXIST 2
	#define OTHER 3

	typedef struct ListPartPlayer* ItemListPlayer;
	struct ListPartPlayer{
		Player* player;
		struct ListPartPlayer* next;
	};

	typedef struct ListPlayer ListPlayer;
	struct ListPlayer{
		int nb;
		ItemListPlayer firstItem;
	};

	ListPlayer* createListPlayer();
	ItemListPlayer createElementPlayer(Player*);
	int addPlayer(ListPlayer*, Player*);
	void removePlayer(ListPlayer*, Player*);
	void removePlayerByName(ListPlayer*, char*);
	Player* getPlayerByName(ListPlayer*, char*);
	Player* getLastPlayer(ListPlayer*);
	bool isPlayerInList(ListPlayer*, Player*);
	bool isPlayerInListByName(ListPlayer*, char*);

	int addPlayer_Item(ItemListPlayer, Player*);
	int removePlayerByName_Item(ItemListPlayer, char*);
	Player* getPlayerByName_Item(ItemListPlayer, char*);
	Player* getLastPlayer_Item(ItemListPlayer);
	bool isPlayerInListByName_Item(ItemListPlayer, char*);
	void saveAllPlayer_Item(ItemListPlayer);

#endif
