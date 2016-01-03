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
	char req_dep[10] = "" ;

	sprintf(req_dep, "1,%d", direction);

	Req = malloc((strlen(req_dep)+1)*sizeof(char)) ;
	strcpy(Req, req_dep) ;

	return Req ;
}

char* Requete_Casse_Bloc (int x, int y) {
	char* Requete ;
	char* req = calloc(30, sizeof(char)) ;

	sprintf(req, "2,%d,%d", x, y);


	Requete = calloc((strlen(req)+1), sizeof(char)) ;

	strcpy(Requete, req) ;

	return Requete ;
}

char* Requete_Pose_Bloc (int x, int y, int index) {
	char* Requete ;
	char* req = calloc(30, sizeof(char)) ;

	// On créé le datagramme
	sprintf(req, "3,%d,%d,%d", x, y, index);


	Requete = calloc((strlen(req)+1), sizeof(char)) ;

	strcpy(Requete, req) ;

	return Requete ;
}

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

void parse_Protocole (Process* p, char* datagramme) {
	Player* player = p->player;
	int taille_data = 0 ;

	// On calcul la taille de datagramme
	taille_data = strlen(datagramme) ;

	// On créé un char[] de la taille de datagramme et on copie datagramme dedans.
	char datagramme_b[taille_data] ;
	strcpy(datagramme_b, datagramme) ;

	// On récupére le type de la requête
	char* type = strtok(datagramme_b, ",") ;

	#ifdef DEBUG
	printf("type : %s\n", type) ;
	#endif

	// Map autour du joueur + position des autres joueurs
	if(!strcmp(type, "1")) {
		char* map = strtok(NULL, ",") ;
		char* nbPlayers = strtok(NULL, ",");
		char* playersChar = strtok(NULL, ",");
		char* invChar = strtok(NULL, ",") ;
		char* fall = strtok(NULL, ",");

		if((map != NULL) && (nbPlayers != NULL) && (atoi(nbPlayers) != 0) && (playersChar != NULL) && (invChar != NULL) && (fall != NULL)){
			#ifdef DEBUG
			printf("Move \n") ;
			#endif
			// Récupération de la map autour
			char* case_actuel ;
			int i = 0 ;
			int j = 0 ;
			block** map_tab = malloc((NB_LIGNE+2*MARGE) * sizeof(block*));
			for(i = 0; i < NB_LIGNE+2*MARGE; i++){
				map_tab[i] = malloc((NB_COLONNE+2*MARGE) * sizeof(block));
			}


			for (i = 0 ; i < NB_LIGNE+2*MARGE ; i++) {
				for (j = 0 ; j < NB_COLONNE+2*MARGE ; j++) {
					if (i+j == 0) {
						case_actuel = strtok(map, "-") ;
					}
					else {
						case_actuel = strtok(NULL, "-") ;
					}
					map_tab[i][j].type = atoi(case_actuel) ;
					map_tab[i][j].back = SKY ;
					#ifdef DEBUG
					printf("%d ", map_tab[i][j].type) ;
					#endif

				}
				#ifdef DEBUG
				printf("\n") ;
				#endif

			}
			block** mapTemp = p->map;
			p->map = map_tab;
			freeMap(mapTemp, SIZE_MAX_X, SIZE_MAX_Y);

			// Récupération des joueurs et de leurs positions
			char* p = strtok(playersChar, "-") ;
			DisplayPlayer* dp = malloc(atoi(nbPlayers) * sizeof(DisplayPlayer));
			i = 0;

			while(p != NULL) {
				char* pseudo = strtok(p, "_") ;
				int pos_X = atoi(strtok(NULL, "_"));
				int pos_Y = atoi(strtok(NULL, "_"));

				#ifdef DEBUG
				printf("Pseudo = %s, pos x = %d, pos y = %d\n", pseudo, pos_X, pos_Y) ;
				#endif

				dp[i].name = pseudo;
				dp[i].x = pos_X;
				dp[i].y = pos_Y;

				p = strtok(NULL, "-") ;
			}

			for(i = 0; i < INV_SIZE; i++){
				block b;
				b.type = atoi(strtok(invChar, "_")) ;
				invCase c;
				c.desc = b;
				c.number = atoi(strtok(NULL, "-")) ;
				player->inventory[i] = c;
			}

			player->falling = atoi(fall);
		}
		else{
			#ifdef DEBUG
			printf("Move but bad arguments\n") ;
			#endif
		}


	}
	// Chat
	else if (!strcmp(type, "2")) {
		char* chat = strtok(NULL, ",") ;
		printf("Chat : %s\n", chat);
	}
	else {
		#ifdef DEBUG
		printf("Erreur, type de protocole inconnu") ;
		#endif
	}
}
