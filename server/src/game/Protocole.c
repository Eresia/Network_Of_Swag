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
			#ifdef DEBUG
			printf("It's a new player\n") ;
			#endif
			int* position = malloc(2*sizeof(int));
			position[0] = gl->map->spawn[0];
			position[1] = gl->map->spawn[1];
			newPlayer->position = position;
		}
		else{
			#ifdef DEBUG
			printf("Player loaded\n") ;
			#endif
		}

		if(addPlayer(gl->listPlayer, newPlayer) == NO_ERROR){
			#ifdef DEBUG
			printf("Parsing - New player added\n") ;
			#endif
			write(desc, &newPlayer, sizeof(newPlayer));
			if(gl->map->map[newPlayer->position[0]][newPlayer->position[1]+1].type == NONE){
				pthread_t thread;
				FallData* data = malloc(sizeof(FallData));
				data->player = newPlayer;
				data->map = gl->map->map;
				newPlayer->falling = true;
				pthread_create(&thread, NULL, fall, data);
			}
		}
		else{
			write(desc, NULL, sizeof(NULL));
		}
	}
	// Déplacement
	else{
		Player* player = getPlayerByName(gl->listPlayer, pseudo);
		if(player == NULL){
			#ifdef DEBUG
			printf("Bad player\n") ;
			#endif
		}
		if (!strcmp(type, "1")) {
			#ifdef DEBUG
			printf("C'est un déplacement vers : %s\n", datagramme_b) ;
			#endif
			Move direction = atoi(datagramme_b);

			switch(direction){
				/*case BOT:
					if(canGoToBlock(player->position[0], player->position[1], 0, 1, gl->map->map)){
						player->position[1]++;
					}
					else{
						#ifdef DEBUG
						printf("Bot direction not possible\n");
						#endif
					}
					break;*/
				case RIGHT:
					if(canGoToBlock(player->position[0], player->position[1], 1, 0, gl->map->map)){
						player->position[0]++;
					}
					else{
						#ifdef DEBUG
						printf("Right direction not possible\n");
						#endif
					}
					break;
				case LEFT:
					if(canGoToBlock(player->position[0], player->position[1], -1, 0, gl->map->map)){
						player->position[0]--;
					}
					else{
						#ifdef DEBUG
						printf("Left direction not possible\n");
						#endif
					}
					break;
				case TOP:
					if(!player->falling && (canGoToBlock(player->position[0], player->position[1], 0, -1, gl->map->map))){
						player->position[1]--;
					}
					else{
						#ifdef DEBUG
						printf("Top direction not possible\n");
						#endif
					}
					break;
				default:
					#ifdef DEBUG
					printf("Direction not know\n");
					#endif
					break;
			}
			if(!player->falling && (gl->map->map[player->position[0]][player->position[1]+1].type == NONE)){
				pthread_t thread;
				FallData* data = malloc(sizeof(FallData));
				data->player = player;
				data->map = gl->map->map;
				player->falling = true;
				pthread_create(&thread, NULL, fall, data);
			}

		}
		// Casse bloc
		else if (!strcmp(type, "2")) {
			int position_x_bloc = atoi(strtok(NULL, ","));
			int position_y_bloc = atoi(strtok(NULL, ","));

			#ifdef DEBUG
			printf("C'est un cassage du bloc de position x : %d, position y : %d\n", position_x_bloc, position_y_bloc) ;
			#endif

			if(canAccesBlock(player->position[0], player->position[1], position_x_bloc, position_y_bloc, gl->map->map, false)){
				block b = gl->map->map[position_x_bloc][position_y_bloc];
				addBlockToInv(player, b);
			}
		}
		// Pose bloc
		else if (!strcmp(type, "3")) {
			int position_x_bloc = atoi(strtok(NULL, ","));
			int position_y_bloc = atoi(strtok(NULL, ","));
			int index = atoi(strtok(NULL, ","));

			#ifdef DEBUG
			printf("C'est un posage du bloc d'index %d à la position x : %d, position y : %d\n", index, position_x_bloc, position_y_bloc) ;
			#endif

			if(canAccesBlock(player->position[0], player->position[1], position_x_bloc, position_y_bloc, gl->map->map, true)){
				if((player->inventory[index].number != 0) && (player->inventory[index].desc.type != NONE)){
					gl->map->map[position_x_bloc][position_y_bloc] = player->inventory[index].desc;
					removeBlockFromInv(player, index);
				}
			}
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
}

// Fonction qui créé le datagramme à envoyer à un joueur.
char* Requete_Maj (char* pseudo, ListPlayer* players, Map* fullMap) {
	//printf("Spawn : %d, %d\n", fullMap->spawn[0], fullMap->spawn[1]);
	block** map = fullMap->map;
	char* requete;
	char* req_dep = calloc(SIZE_MESSAGE_MAX + 1, sizeof(char));
	//char map_char[(((NB_LIGNE+2*MARGE)*(NB_COLONNE+2*MARGE))*2)+1] ;
	char* map_char = calloc(SIZE_MESSAGE_MAX, sizeof(char));
	char* posPlayers = calloc(SIZE_MESSAGE_MAX, sizeof(char));
	char* inv = calloc(SIZE_MESSAGE_MAX, sizeof(char));
	int i = 0 ;
	int j = 0 ;

	Player* player = getPlayerByName(players, pseudo);
	int nbPlayers = 0;

	ItemListPlayer item = players->firstItem;
	while(item != NULL){
		if(playerIsVisible(player, item->player)){
			sprintf(posPlayers, "%s%s_%d_%d-", posPlayers, item->player->name, item->player->position[0], item->player->position[1]);
			nbPlayers++;
		}
		item = item->next;
	}

	for (i = player->position[0]-((NB_LIGNE+2*MARGE)/2) ; i < player->position[0]+((NB_LIGNE+2*MARGE)/2)  ; i++) {
		for (j = player->position[1]-((NB_COLONNE+2*MARGE)/2); j < player->position[1]+((NB_COLONNE+2*MARGE)/2) ; j++) {
			if((i >= 0) && (i < SIZE_MAX_X) && (j >= 0) && (j < SIZE_MAX_Y)){
				sprintf(map_char, "%s%d-", map_char, map[i][j].type);
			}
		}
	}

	for(i = 0; i < INV_SIZE; i++){
		sprintf(inv, "%s%d_%d-", inv, player->inventory[i].desc.type, player->inventory[i].number);
	}

	// On créé le datagramme
	sprintf(req_dep, "1,%s,%d,%s,%s,%d", map_char, nbPlayers, posPlayers, inv, player->falling);

	requete = malloc((strlen(req_dep)+1)*sizeof(char)) ;
	strcpy(requete, req_dep) ;

	/*printf("SizeMax : %d\n", SIZE_MESSAGE_MAX);
	printf("Map : %d\n", (int) strlen(map_char));
	printf("nbPlayer : %d\n", nbPlayers);
	printf("posPlayers : %s - taille : %d\n", posPlayers, (int) strlen(posPlayers));
	printf("inv : %s - taille : %d\n", inv, (int) strlen(inv));
	printf("All : %d\n", (int) strlen(requete));*/

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
