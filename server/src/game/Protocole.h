/**

	[SERVEUR]
	Fonctions qui parse un datagramme pour en ressortir les données.

**/

#ifndef PROTOCOLE
#define PROTOCOLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Threads include
#include <pthread.h>

#include "../../../data/src/usefull/Usefull.h"
#include "Gameloop.h"

void Parse_Protocole (char*) ;
void* processing(void*);

#endif
