#include "../game/Server.h"

/*Server arguments :
	1 : Port of communication
	2 : Port of UE

*/
int main(int argc, char** argv){

	Server* server;
	Gameloop gl;
	ServerNetwork sn;

	if(argc != 2){
		printf("Not expected arguments");
		return BAD_NUMBER_OF_ARGUMENTS;
	}

	sn.port = atoi(argv[1]);

	if(sn.port == 0){
		printf("Number expected for port");
		return INCORRECT_ARGUMENT;
	}

	server = malloc(sizeof(Server));
	sn.connectedClient = 0;
	sn.clients = malloc(NB_CLIENT_MAX*sizeof(ClientNetwork*));

	server->gl = gl;
	server->sn = sn;

	//Launch server
	//Launch shell

	printf("Je suis un serveur\n");


	return NO_ERROR;



}
