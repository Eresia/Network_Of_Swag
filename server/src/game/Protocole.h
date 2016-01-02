#ifndef PROTOCOLE
#define PROTOCOLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Threads include
#include <pthread.h>

#include "../../../data/src/usefull/Usefull.h"
#include "../../../data/src/map/Map.h"
#include "../../../data/src/player/ListPlayer.h"
#include "Gameloop.h"

#define NB_LIGNE 3
#define NB_COLONNE 3
#define MARGE 3

#define SIZE_MESSAGE_MAX 1500
#define FORBIDEN_CHAR " \n"

void parse_Protocole (char*, char*, Gameloop*, int) ;
char* Requete_Maj (Player*, Map*) ;
char* Requete_Chat (char* messages) ;

#endif
