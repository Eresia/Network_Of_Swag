/**
	Parse
	&
	Traitement Protocoles Serveur -> Client

	
	Format du protocole : "[Type de requête],[Data en fonction de la requête (n bits)]"


	Requêtes :

	0 = Libre
	1 = Pos joueur + Map autour
	2 = Chat
	3 = Message de déco (déco propre)


	Format Data en fonction des requêtes :


	1 = Maj = Pos joueur + Map autour

	[Pos_x],[Pos_y],[Map_autour]
	
	Exemple map autour :
	1 2 3
	4 5 6	=> 1-2-3-4-5-6-7-8-9
	7 8 9


	2 = Chat

	[Messages]


	3 = Message de déco

	[ ? ]



**/

#include "Protocole.h"


// Fonction qui parse un datagramme RECU
void Parse_Protocole (char* datagramme) {
	int taille_data = 0 ;

	// Calcul de la taille de datagramme ;
	taille_data = strlen(datagramme) ;

	// On déclare un char[] de la taille de datagramme et on copie datagramme dedans.
	char datagramme_b[taille_data] ;
	strcpy (datagramme_b, datagramme) ;

	// On récupère le type de requête
	char* type = strtok(datagramme_b, ",") ;
	printf("type : %s\n\n", datagramme_b) ;

	// Déplacement
	if (!strcmp(type, "1")) {
		char* direction = strtok(NULL, ",");
		printf("C'est un déplacement vers : %s\n", direction) ;
	}
	// Casse bloc
	else if (!strcmp(type, "2")) {
		char* position_x_bloc = strtok(NULL, ",") ;
		char* position_y_bloc = strtok(NULL, ",") ;

		printf("C'est un cassage du bloc de position x : %s, position y : %s\n", position_x_bloc, position_y_bloc) ;
	}
	// Pose bloc
	else if (!strcmp(type, "3")) {
		char* position_x_bloc = strtok(NULL, ",") ;
		char* position_y_bloc = strtok(NULL, ",") ;

		printf("C'est un posage du bloc de position x : %s, position y : %s\n", position_x_bloc, position_y_bloc) ;
	}
	// Message
	else if (!strcmp(type, "4")) {
		char* message = strtok(NULL, ",");
		printf("C'est un message chat : %s\n", message) ;
	}
	else {
		printf("Erreur, type de requête inconnu\n") ;
	}
}

// Fonction qui créé le datagramme à envoyer à un joueur.
char* Requete_Maj (int x, int y, int Map[NB_LIGNE+2*MARGE][NB_COLONNE+2*MARGE]) {
	char* Requete ;
	char req_dep[1500] = "" ;
	char x_position[10] ;
	char y_position[10] ;
	//char map_char[(((NB_LIGNE+2*MARGE)*(NB_COLONNE+2*MARGE))*2)+1] ;
	char map_char[1400] ;
	char chiffre[1400] ;
	int i = 0 ;
	int j = 0 ;

	// Récupération de x comme chaine de caractères.
	sprintf(x_position, "%d", x);

	// Récupération de y comme chaine de caractères.
	sprintf(y_position, "%d", y);
	
	for (i = 0 ; i < (NB_LIGNE+2*MARGE) ; i++) {
		for (j = 0 ; j < (NB_COLONNE+2*MARGE) ; j++) {
			sprintf(chiffre, "%d", Map[i][j]);

			strcat(map_char, chiffre) ;

			// On ajoute "-" après le chiffre sauf pour le dernier
			if (i+j != (((NB_LIGNE+2*MARGE)*2)-2)) {
				strcat(map_char, "-") ;
			}
		}
	}
 	
	// On créé le datagramme
	strcat(req_dep, "1,");
	strcat(req_dep, x_position);
	strcat(req_dep, ",");
	strcat(req_dep, y_position);
	strcat(req_dep, ",");
	strcat(req_dep, map_char) ;
	
	Requete = malloc((strlen(req_dep)+1)*sizeof(char)) ;
	strcpy(Requete, req_dep) ;

	return Requete ;	
}

// Fonction qui envoie le chat.
char* Requete_Message (char* message) {	
	char* Requete ;
	char req_mess[1503] = "" ;

	// On créé le protocole.
	strcat(req_mess, "4,");
	strcat(req_mess, message) ;

	Requete = calloc((strlen(req_mess)+1), sizeof(char)) ;
	strcpy(Requete, req_mess) ;

	return Requete ;
}
