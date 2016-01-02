#ifndef PROTOCOLE
#define PROTOCOLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Threads include
#include <pthread.h>

#include "../../../data/src/usefull/Usefull.h"
#include "Gameloop.h"

#define NB_LIGNE 3
#define NB_COLONNE 3
#define MARGE 3

void parse_Protocole (char*, char*) ;
char* Requete_Maj (int x, int y, int Map[NB_LIGNE+2*MARGE][NB_COLONNE+2*MARGE]) ;
char* Requete_Chat (char* messages) ;
void* processing(void*);
char* waitMessage(int, int, int);

#endif
