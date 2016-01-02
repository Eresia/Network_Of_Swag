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

/*int main (int argc, char* argv[]) {
	char* req_deplacement ;
	char* req_bloc ;
	char* req_pose_bloc ;
	char* req_message ;

	req_deplacement = Requete_Deplacement_Envoi(2) ;
	printf("%s\n\n", req_deplacement) ;

	req_bloc = Requete_Casse_Bloc(324, 758) ;
	printf("%s\n\n", req_bloc) ;

	req_pose_bloc = Requete_Pose_Bloc(24,1067);
	printf("%s\n\n", req_pose_bloc) ;

	req_message = Requete_Message("Je suis un test du protocole d'envoi de message");
	printf("%s\n\n", req_message) ;

	char* requeteTest = "1,0-1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31-32-33-34-35-36-37-38-39-40-41-42-43-44-45-46-47-48-49-50-51-52-53-54-55-56-57-58-59-60-61-62-63-64-65-66-67-68-69-70-71-72-73-74-75-76-77-78-79-80,Alex_34_68,Bruno_2_92,Bastien_154_292";

	parse_Protocole (requeteTest) ;

	return 0 ;
}*/

/**
	Protocole de déplacement (en fonction de la direction)
	2 : Bas
	4 : Gauche
	6 : Droite
	8 : Haut
**/
char* Requete_Deplacement_Envoi (int direction) {
	char* Req ;
	char req_dep[10] = "" ;
	char c_direction[2] ;

	// On transforme le int en chaîne de caractères.
	sprintf(c_direction, "%d", direction);

	strcat(req_dep, "1,");
	strcat(req_dep, c_direction);

	Req = malloc((strlen(req_dep)+1)*sizeof(char)) ;
	strcpy(Req, req_dep) ;

	return Req ;
}

char* Requete_Casse_Bloc (int x, int y) {
	char* Requete ;
	char req[30] = "" ;
	char x_position[10] ;
	char y_position[10] ;

	// Récupération de x comme chaine de caractères.
	sprintf(x_position, "%d", x);

	// Récupération de y comme chaine de caractères.
	sprintf(y_position, "%d", y);

	// On créé le datagramme
	strcat(req, "2,") ;
	strcat(req, x_position) ;
	strcat(req, ",") ;
	strcat(req, y_position) ;


	Requete = calloc((strlen(req)+1), sizeof(char)) ;

	strcpy(Requete, req) ;

	return Requete ;
}

char* Requete_Pose_Bloc (int x, int y) {
	char* Requete ;
	char req[30] = "" ;
	char x_position[10] ;
	char y_position[10] ;

	// Récupération de x comme chaine de caractères.
	sprintf(x_position, "%d", x);

	// Récupération de y comme chaine de caractères.
	sprintf(y_position, "%d", y);

	// On créé le datagramme
	strcat(req, "3,") ;
	strcat(req, x_position) ;
	strcat(req, ",") ;
	strcat(req, y_position) ;


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

void parse_Protocole (char* datagramme) {
	int taille_data = 0 ;
	char* saveptr1 ;
	
	// On calcul la taille de datagramme
	taille_data = strlen(datagramme) ;

	// On créé un char[] de la taille de datagramme et on copie datagramme dedans.
	char datagramme_b[taille_data] ;
	strcpy(datagramme_b, datagramme) ;

	// On récupére le type de la requête
	char* type = strtok_r(datagramme_b, ",", &saveptr1) ;

	#ifdef DEBUG
	printf("type : %s\n", type) ;
	#endif

	// Map autour du joueur + position des autres joueurs
	if(!strcmp(type, "1")) {
		char* map = strtok_r(NULL, ",", &saveptr1) ;
		
		// Récupération de la map autour
		char* case_actuel ;
		int i = 0 ;
		int j = 0 ;	
		int map_tab[NB_LIGNE+2*MARGE][NB_COLONNE+2*MARGE] ;
			
		for (i = 0 ; i < NB_LIGNE+2*MARGE ; i++) {
			for (j = 0 ; j < NB_COLONNE+2*MARGE ; j++) {
				if (i+j == 0) {				
					case_actuel = strtok(map, "-") ;
				}
				else {
					case_actuel = strtok(NULL, "-") ;	 
				}
				map_tab[i][j] = atoi(case_actuel) ;
				#ifdef DEBUG
				printf("%d ", map_tab[i][j]) ;
				#endif

			}
			#ifdef DEBUG
			printf("\n") ;
			#endif

		}

		// Récupération des joueurs et de leurs positions
		char* pseudo ;
		int pos_X ;
		int pos_Y ;
		char* player = strtok_r(NULL, ",", &saveptr1) ;

		do {
			pseudo = strtok(player, "_") ;
			pos_X = atoi(strtok(NULL, "_")) ;
			pos_Y = atoi(strtok(NULL, "_")) ;
			
			#ifdef DEBUG
			printf("Pseudo = %s, pos x = %d, pos y = %d\n", pseudo, pos_X, pos_Y) ;
			#endif

			player = strtok_r(NULL, ",", &saveptr1) ;
		}while (player != NULL);
		
	}
	// Chat
	else if (!strcmp(type, "2")) {
		char* chat = strtok_r(NULL, ",", &saveptr1) ;
	}
	else {
		#ifdef DEBUG
		printf("Erreur, type de protocole inconnu") ;
		#endif
	}
}

