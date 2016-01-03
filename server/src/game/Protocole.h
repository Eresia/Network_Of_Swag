#ifndef PROTOCOLE
#define PROTOCOLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

//Threads include
#include <pthread.h>

#include "../../../data/src/usefull/Usefull.h"
#include "../../../data/src/commonProtocol/CommonProtocol.h"
#include "../../../data/src/map/Map.h"
#include "../../../data/src/player/ListPlayer.h"
#include "Gameloop.h"

typedef struct FallData FallData;
struct FallData{
	Player* player;
	block** map;
};

void parse_Protocole (char*, char*, Gameloop*, int) ;
char* Requete_Maj (char*, ListPlayer*, Map*) ;
char* Requete_Chat (char* messages) ;
bool playerIsVisible(Player*, Player*);
bool canGoToBlock(int, int, int, int, block**);
bool canAccesBlock(int, int, int, int, block**, bool);
void* fall(void*);

#endif
