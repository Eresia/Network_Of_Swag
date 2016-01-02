#include "Protocole.h"
#include "Server.h"

void parse_Protocole (char* pseudo, char* datagramme, Gameloop* gl, int desc) {
	int taille_data = 0 ;

	#ifdef DEBUG
	printf("Parsing - pseudo : %s - datagram : %s\n", pseudo, datagramme);
	#endif

	// Calcul de la taille de datagramme ;
	taille_data = strlen(datagramme) ;

	// On déclare un char[] de la taille de datagramme et on copie datagramme dedans.
	char datagramme_b[taille_data] ;
	strcpy (datagramme_b, datagramme) ;

	// On récupère le type de requête
	char* type = strtok(datagramme_b, ",") ;
	#ifdef DEBUG
	printf("type : %s\n\n", datagramme_b) ;
	#endif

	// Nouveau joueur
	if (!strcmp(type, "-1")) {
		#ifdef DEBUG
		printf("Parsing - New Player : %s\n", pseudo) ;
		#endif
		Player* newPlayer = loadPlayer(pseudo);
		if(newPlayer->position == NULL){
			int* position = malloc(2*sizeof(int));
			position[0] = gl->map->spawn[0];
			position[1] = gl->map->spawn[1];
			newPlayer->position = position;
		}

		if(addPlayer(gl->listPlayer, newPlayer) == NO_ERROR){
			#ifdef DEBUG
			printf("Parsing - New player added\n") ;
			#endif
			write(desc, &newPlayer, sizeof(newPlayer));
		}
		else{
			write(desc, NULL, sizeof(NULL));
		}
	}
	// Déplacement
	else if (!strcmp(type, "1")) {
		char* direction = strtok(NULL, ",");

		#ifdef DEBUG
		printf("C'est un déplacement vers : %s\n", direction) ;
		#endif
	}
	// Casse bloc
	else if (!strcmp(type, "2")) {
		char* position_x_bloc = strtok(NULL, ",") ;
		char* position_y_bloc = strtok(NULL, ",") ;

		#ifdef DEBUG
		printf("C'est un cassage du bloc de position x : %s, position y : %s\n", position_x_bloc, position_y_bloc) ;
		#endif
	}
	// Pose bloc
	else if (!strcmp(type, "3")) {
		char* position_x_bloc = strtok(NULL, ",") ;
		char* position_y_bloc = strtok(NULL, ",") ;

		#ifdef DEBUG
		printf("C'est un posage du bloc de position x : %s, position y : %s\n", position_x_bloc, position_y_bloc) ;
		#endif
	}
	// Message
	else if (!strcmp(type, "4")) {
		char* message = strtok(NULL, ",");

		#ifdef DEBUG
		printf("C'est un message chat : %s\n", message) ;
		#endif
	}
	else {
		#ifdef DEBUG
		printf("Erreur, type de requête inconnu\n") ;
		#endif
	}
}

// Fonction qui créé le datagramme à envoyer à un joueur.
char* Requete_Maj (char* pseudo, ListPlayer* players, Map* fullMap) {
	//printf("Spawn : %d, %d\n", fullMap->spawn[0], fullMap->spawn[1]);
	block** map = fullMap->map;
	char* requete ;
	char* req_dep = calloc(SIZE_MESSAGE_MAX + 1, sizeof(char));
	//char map_char[(((NB_LIGNE+2*MARGE)*(NB_COLONNE+2*MARGE))*2)+1] ;
	char* map_char = calloc(1400, sizeof(char));
	char* chiffre = calloc(1400, sizeof(char));
	char* posPlayers = calloc(1400, sizeof(char));
	int i = 0 ;
	int j = 0 ;

	Player* player = getPlayerByName(players, pseudo);

	ItemListPlayer item = players->firstItem;
	while(item != NULL){
		if(playerIsVisible(player, item->player)){
			sprintf(posPlayers, "%s%s_%d_%d-", posPlayers, item->player->name, item->player->position[0], item->player->position[1]);
		}
		item = item->next;
	}

	for (i = player->position[0]-((NB_LIGNE+2*MARGE)/2) ; i < player->position[0]+((NB_LIGNE+2*MARGE)/2)  ; i++) {
		for (j = player->position[1]-((NB_COLONNE+2*MARGE)/2); j < player->position[1]+((NB_COLONNE+2*MARGE)/2) ; j++) {
			if((i >= 0) && (i < SIZE_MAX_X) && (j >= 0) && (j < SIZE_MAX_Y)){
				sprintf(chiffre, "%d", map[i][j].type);

				strcat(map_char, chiffre) ;

				// On ajoute "-" après le chiffre sauf pour le dernier
				if (i+j != (((NB_LIGNE+2*MARGE)*2)-2)) {
					strcat(map_char, "-") ;
				}
			}
		}
	}

	// On créé le datagramme
	sprintf(req_dep, "%s%s%s%c%s", req_dep, "1,", map_char, ',', posPlayers);

	requete = malloc((strlen(req_dep)+1)*sizeof(char)) ;
	strcpy(requete, req_dep) ;

	return requete ;
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

bool playerIsVisible(Player* player, Player* other){
	if(other->position[0] < player->position[0]-((NB_LIGNE+2*MARGE)/2)){
		return false;
	}
	if(other->position[0] >= player->position[0]+((NB_LIGNE+2*MARGE)/2)){
		return false;
	}
	if(other->position[1] < player->position[1]-((NB_COLONNE+2*MARGE)/2)){
		return false;
	}
	if(other->position[1] >= player->position[1]+((NB_COLONNE+2*MARGE)/2)){
		return false;
	}
	return true;
}
