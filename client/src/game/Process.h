#ifndef PROCESS
#define PROCESS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../../../data/src/usefull/Usefull.h"
#include "../../../data/src/commonProtocol/CommonProtocol.h"
#include "../../../data/src/map/Map.h"
#include "../../../data/src/player/Player.h"

typedef struct DisplayPlayer DisplayPlayer;
struct DisplayPlayer{
	char* name;
	int x;
	int y;
};

typedef struct Process Process;
struct Process{
		block** map;
		Player* player;
		int nbPlayers;
		DisplayPlayer* players;
		int* desc;
};

void move(Player*, Move, block**, int);
void breakBlock(Player*, int, int, block**, int);
void putBlock(Player*, int, int, int, block**, int);

#endif
