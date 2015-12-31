#include "launch_server.h"
#include "../../../data/src/map/Map.h"

/*Server arguments :
	1 : Port of communication
*/
int main(int argc, char** argv){

	Server server;
	Gameloop gl;
	ServerNetwork sn;
	pthread_t thread_gameloop, thread_network, thread_shell;

	if(argc != 2){
		printf("Not expected arguments\n");
		return BAD_NUMBER_OF_ARGUMENTS;
	}

	sn.port = atoi(argv[1]);

	if(sn.port == 0){
		printf("Number expected for port\n");
		return INCORRECT_ARGUMENT;
	}

	gl.isStopped = false;
	gl.thread = &thread_gameloop;

	sn.connectedClient = 0;
	sn.clients = malloc(NB_CLIENT_MAX*sizeof(ClientNetwork*));
	sn.thread = &thread_network;

	//server = malloc(sizeof(Server));
	server.gl = gl;
	server.sn = sn;

	sem_unlink("/semGl");

	pthread_create(&thread_gameloop, NULL, launch_gameloop, &server);
	pthread_create(&thread_network, NULL, launch_network, &server);
	pthread_create(&thread_shell, NULL, launch_shell, &server);
	pthread_join(thread_gameloop, NULL);

	printf("Serveur stopped\n");


	return NO_ERROR;



}
