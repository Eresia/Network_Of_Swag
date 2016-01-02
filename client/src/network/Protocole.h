/**

	Traitement Protocoles Client -> Serveur


	Format du protocole : "[Type de requête],[Data en fonction de la requête (n bits)]"


	Requêtes :

	0 = Libre
	1 = Déplacement
	2 = Casse bloc
	3 = Pose bloc
	4 = Message chat
	5 = Message de déco (déco propre)


	Format Data en fonction des requêtes :


	1 = Déplacement

	[Direction]


	2 = Casse bloc

	[Position x du bloc],[Position y du bloc]


	3 = Pose bloc

	[Position x],[Position y]


	4 = Message chat

	[Message]


	5 = Message de déco

	[ ? ]



**/

#ifndef PROTOCOLE
#define PROTOCOLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NB_LIGNE 3
#define NB_COLONNE 3
#define MARGE 3

char* Requete_Deplacement_Envoi (int direction) ;
char* Requete_Casse_Bloc (int x, int y) ;
char* Requete_Pose_Bloc (int x, int y) ;
char* Requete_Message (char* message) ;
void parse_Protocole (char* datagramme) ;

#endif
