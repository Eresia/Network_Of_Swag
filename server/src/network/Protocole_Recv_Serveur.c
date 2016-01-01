/**

	[SERVEUR]
	Fonctions qui parse un datagramme pour en ressortir les données.

**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Parse_Protocole (char* datagramme) ;

int main () {

	Parse_Protocole("2,324,758") ;


	return 0 ;
}

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
