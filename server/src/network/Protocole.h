/**

	[SERVEUR]
	Fonctions qui parse un datagramme pour en ressortir les données.

**/

#ifndef PROTOCOLE
#define PROTOCOLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NB_LIGNE 3
#define NB_COLONNE 3
#define MARGE 3

void Parse_Protocole (char* datagramme) ;
char* Requete_Maj (int x, int y, int Map[NB_LIGNE+2*MARGE][NB_COLONNE+2*MARGE]) ;
char* Requete_Chat (char* messages) ;

#endif
