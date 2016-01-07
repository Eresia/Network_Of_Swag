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


#include "Protocole.h"

char* Requete_Deplacement_Envoi (int direction) {
	char* Req ;
	char* req_dep = calloc(10, sizeof(char)) ;

	sprintf(req_dep, "1,%d", direction);

	Req = malloc((strlen(req_dep)+1)*sizeof(char)) ;
	strcpy(Req, req_dep) ;

	#ifdef DEBUG
	printf("Move request : %d\n", direction);
	#endif

	return Req ;
}

char* Requete_Casse_Bloc (int x, int y) {
	char* Requete ;
	char* req = calloc(30, sizeof(char)) ;

	sprintf(req, "2,%d,%d", x, y);


	Requete = calloc((strlen(req)+1), sizeof(char)) ;

	strcpy(Requete, req) ;

	#ifdef DEBUG
	printf("Block break request : %d %d\n", x, y);
	#endif

	return Requete ;
}

char* Requete_Pose_Bloc (int x, int y, int index) {
	char* Requete ;
	char* req = calloc(30, sizeof(char)) ;

	/*On créé le datagramme*/
	sprintf(req, "3,%d,%d,%d", x, y, index);


	Requete = calloc((strlen(req)+1), sizeof(char)) ;

	strcpy(Requete, req) ;

	#ifdef DEBUG
	printf("Block put request : %d %d - item %d\n", x, y, index);
	#endif

	return Requete ;
}

char* Requete_Message (char* message) {
	char* Requete ;
	char* req_mess = calloc(1400, sizeof(char)) ;

	/*On créé le protocole.*/
	sprintf(req_mess, "4,%s", message);

	Requete = calloc((strlen(req_mess)+1), sizeof(char)) ;
	strcpy(Requete, req_mess) ;

	return Requete ;
}

void parse_Protocole (Process* p, char* datagramme) {
	Player* player;
	int taille_data;
	char* datagramme_b;
	char* type;

	player = p->player;

	/*On calcul la taille de datagramme*/
	taille_data = strlen(datagramme) ;
	datagramme_b = malloc(taille_data* sizeof(char));

	/*On créé un char[] de la taille de datagramme et on copie datagramme dedans.*/
	strcpy(datagramme_b, datagramme) ;

	/*On récupére le type de la requête*/
	type = strtok(datagramme_b, ",") ;

	/*#ifdef DEBUG
	printf("type : %s\n", type) ;
	#endif*/

	/* Map autour du joueur + position des autres joueurs*/
	if(!strcmp(type, "1")) {
		char* map;
		char* nbPlayersChar;
		char* playersChar;
		char* invChar;
		char* fall;

		map = strtok(NULL, ",") ;
		nbPlayersChar = strtok(NULL, ",");
		playersChar = strtok(NULL, ",");
		invChar = strtok(NULL, ",") ;
		fall = strtok(NULL, ",");

		if((map != NULL) && (nbPlayersChar != NULL) && (atoi(nbPlayersChar) != 0) && (playersChar != NULL) && (invChar != NULL) && (fall != NULL)){
			/*#ifdef DEBUG
			printf("Reception \n") ;
			#endif*/
			/* Récupération de la map autour*/
			char* case_actuel ;
			int i;
			int j;
			bool first = true;
			int nbPlayers;
			bool isOk, byPlayer;
			DisplayPlayer* dp;
			block** map_tab;
			map_tab = malloc(SIZE_MAX_X * sizeof(block*));
			for(i = 0; i < SIZE_MAX_X; i++){
				map_tab[i] = malloc(SIZE_MAX_Y * sizeof(block));
			}

			/* Récupération des joueurs et de leurs positions*/
			nbPlayers = atoi(nbPlayersChar);
			isOk = true;
			byPlayer = false;
			dp = malloc((nbPlayers) * sizeof(DisplayPlayer));
			i = 0;
			first = true;

			for(i = 0; i < nbPlayers; i++) {
				char* pseudo, *pos_XChar, *pos_YChar;
				if(first){
					pseudo = strtok(playersChar, "_") ;
					first = false;
				}
				else{
					pseudo = strtok(NULL, "_");
				}
				pos_XChar = strtok(NULL, "_");
				pos_YChar = strtok(NULL, "-");
				if((pseudo != NULL) && (pos_XChar != NULL) && (pos_YChar != NULL)){

					int pos_X;
					int pos_Y;

					pos_X = atoi(pos_XChar);
					pos_Y = atoi(pos_YChar);

					/*#ifdef DEBUG
					printf("Pseudo = %s, pos x = %d, pos y = %d\n", pseudo, pos_X, pos_Y) ;
					#endif*/

					if(strcmp(p->player->name, pseudo) == 0){
						player->position[0] = pos_X;
						player->position[1] = pos_Y;
						byPlayer = true;
					}
					else{
						int posInList;
						if(!byPlayer){
							posInList = i;
						}
						else{
							posInList = i-1;
						}
						dp[posInList].name = pseudo;
						dp[posInList].x = pos_X;
						dp[posInList].y = pos_Y;
					}
				}
				else{
					printf("Bad parameters (update)\n");
					isOk = false;
					break;
				}
			}
			if(isOk){
				bool begin;
				p->players = dp;
				p->nbPlayers = nbPlayers;
				isOk = true;
				first = true;
				for (i = player->position[0]-((NB_LIGNE+2*MARGE)/2) ; i < player->position[0]+((NB_LIGNE+2*MARGE)/2)  ; i++) {
					for (j = player->position[1]-((NB_COLONNE+2*MARGE)/2); j < player->position[1]+((NB_COLONNE+2*MARGE)/2) ; j++) {
						if((i >= 0) && (i < SIZE_MAX_X) && (j >= 0) && (j < SIZE_MAX_Y)){
							if (first) {
								case_actuel = strtok(map, "-") ;
								first = false;
							}
							else {
								case_actuel = strtok(NULL, "-") ;
							}
							if(case_actuel != NULL){
								map_tab[i][j].type = atoi(case_actuel) ;
								map_tab[i][j].back = SKY ;
								/*#ifdef DEBUG
								printf("%d ", map_tab[i][j].type) ;
								#endif*/
							}
							else{
								isOk = false;
								break;
							}
						}

					}
					if(!isOk){
						break;
					}
					/*#ifdef DEBUG
					printf("\n") ;
					#endif*/

				}
				if(isOk){
					p->map = map_tab;
				}

				/*#ifdef DEBUG
				printf("Remplissage inv : %s\n", invChar) ;
				#endif*/
				begin = true;

				for(i = 0; i < INV_SIZE; i++){
					block b;
					invCase c;
					if(begin){
						b.type = atoi(strtok(invChar, "_")) ;
						begin = false;
					}
					else{
						b.type = atoi(strtok(NULL, "_")) ;
					}
					b.back = SKY;
					c.desc = b;
					c.number = atoi(strtok(NULL, "-")) ;
					player->inventory[i] = c;
				}

				player->falling = atoi(fall);
				/*#ifdef DEBUG
				printf("Reception successfull\n") ;
				#endif*/
			}
		}
		else{
			#ifdef DEBUG
			printf("Reception but bad arguments map : %p,  nbPlayersChar : %p, playersChar : %p, invChar : %p, fall : %p:\n\n", map, nbPlayersChar, playersChar, invChar, fall);
			#endif
		}
	}
	/* Chat*/
	else if (!strcmp(type, "2")) {
		char* chat;
		chat = strtok(NULL, ",") ;
		if(chat != NULL){
			printf("%s\n", chat);
		}
	}
	else {
		#ifdef DEBUG
		printf("Erreur, type de protocole inconnu : %s\n", type) ;
		#endif
	}
}
