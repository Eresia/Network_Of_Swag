#include "ServerNetwork.h"

int launch_server(int port){




	return NO_ERROR;
}

//Fonction qui lance l'écoute du serveur
int begin_listen(SOCKET* server, SOCKADDR_IN* info, int port){

	*server = socket(AF_INET, SOCK_STREAM, 0);
	if(*server == INVALID_SOCKET)
	{
		#ifdef DEBUG
		printf("Error in socket creation\n");
		#endif
		return OTHER_ERROR;
	}

	#ifdef DEBUG
	printf("Socket creation successfull\n");
	#endif

	//On assigne les informations de connexions au serveur
	info->sin_addr.s_addr = htonl(INADDR_ANY); /* Accept any adress */
	info->sin_family = AF_INET;
	info->sin_port = htons(port);

	//On bind le serveur sur le port s'il est disponible
	if(bind (*server, (SOCKADDR *) info, sizeof(*info)) == SOCKET_ERROR)
	{
		#ifdef DEBUG
		printf("Socket can't be bind\n");
		#endif
		return OTHER_ERROR;
	}

	#ifdef DEBUG
	printf("Socket binded\n");
	#endif

	//On commence à écouter
	if(listen(*server, NB_CLIENT_MAX) == SOCKET_ERROR)
	{
		#ifdef DEBUG
		printf("Error while listen\n");
		#endif
		return OTHER_ERROR;
	}

	#ifdef DEBUG
	printf("Server is listening\n");
	#endif

	return NO_ERROR;
}
