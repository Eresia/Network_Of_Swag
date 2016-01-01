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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* Requete_Deplacement_Envoi (int direction) ;
char* Requete_Casse_Bloc (int x, int y) ;
char* Requete_Pose_Bloc (int x, int y) ;
char* Requete_Message (char* message) ;

int main (int argc, char* argv[]) {
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

	return 0 ;
}

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
