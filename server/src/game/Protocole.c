#include "Protocole.h"

void parse_Protocole (char* pseudo, char* datagramme) {
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

	// Déplacement
	if (!strcmp(type, "1")) {
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

void* processing(void* gl_void){
	Gameloop* gl = (Gameloop*) gl_void;
	char* buff;
	bool stopServer;

	do{
		buff = waitMessage(gl->desc[0], 1, 0);

		if(buff != NULL){
			char* pseudo = calloc(11, sizeof(char));
			char* cmd = calloc(SIZE_MESSAGE_MAX + 1, sizeof(char));
			pseudo = strtok(buff, ",");
			if(pseudo == NULL){
				#ifdef DEBUG
				printf("Bad string - pseudo (Gameloop)\n") ;
				#endif
			}
			else{
				cmd = strtok(NULL, "");
				if(cmd == NULL){
					#ifdef DEBUG
					printf("Bad string - cmd (Gameloop)\n") ;
					#endif
				}
				else{
					parse_Protocole(pseudo, cmd);
				}
			}
		}

		pthread_mutex_lock(gl->stopMutex);
		stopServer = gl->isStopped;
		pthread_mutex_unlock(gl->stopMutex);
		usleep(50);
	}while(!stopServer);

	pthread_exit(NULL);
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

char* waitMessage(int out, int secTimeout, int uSecTimeout){
	int iResult;
	fd_set rfds;
	struct timeval tv;
	int n;
	char* buff = calloc(SIZE_MESSAGE_MAX + 11, sizeof(char));

	FD_ZERO(&rfds);
	FD_SET(out, &rfds);

	tv.tv_sec = (long) secTimeout;
	tv.tv_usec = (long) uSecTimeout;

	iResult = select(out + 1, &rfds, (fd_set *) 0, (fd_set *) 0, &tv);
	if(iResult > 0)
	{
		if((n = read(out, buff, SIZE_MESSAGE_MAX + 11)) < 0){
			#ifdef DEBUG
			printf("Gameloop : Message not received\n");
			#endif
			return NULL;
		}
		else{
			buff[n] = '\0';
			#ifdef DEBUG
			printf("Gameloop : Messages : %s\n", buff);
			#endif
			return buff;
		}
	}
	else
	{
		return NULL;
	}
}
