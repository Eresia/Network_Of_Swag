#ifndef LIST_PLAYER
	#define LIST_PLAYER

	#include <stdlib.h>
	#include <stdio.h>

	#include "../../../data/src/usefull/Usefull.h"
	#include "player.h"

	#define ELM_ALREADY_EXIST 1
	#define ELM_NOT_EXIST 2
	#define OTHER 3

	typedef struct ListPartPlayer* ItemListPlayer;
	struct ListPartPlayer{
		player* player;
		struct ListPartPlayer* next;
	};

	typedef struct ListPlayer ListPlayer;
	struct ListPlayer{
		int nb;
		ItemListPlayer firstItem;
	};

	ListPlayer* createListPlayer();
	ItemListPlayer createElementPlayer(player*);
	void addPlayer(ListPlayer*, player*);
	void removePlayer(ListPlayer*, player*);
	void removePlayerByName(ListPlayer*, char*);
	player* getPlayerByName(ListPlayer*, char*);
	player* getLastPlayer(ListPlayer*);
	bool isPlayerInList(ListPlayer*, player*);
	bool isPlayerInListByName(ListPlayer*, char*);
	void closeAllPlayer(ListPlayer*);

	int addPlayer_Item(ItemListPlayer, player*);
	int removePlayerByName_Item(ItemListPlayer, char*);
	player* getPlayerByName_Item(ItemListPlayer, char*);
	player* getLastPlayer_Item(ItemListPlayer);
	bool isPlayerInListByName_Item(ItemListPlayer, char*);
	void closeAllPlayer_Item(ItemListPlayer);

#endif
