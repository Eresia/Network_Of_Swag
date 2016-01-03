#include "launch_client.h"

int main(int argc, char** argv){

	pthread_t graphic, network;
	Process process;
	Client client;
	int* descProc = malloc(2*sizeof(int));
	int port;

	if(argc != 4){
		printf("Not expected arguments\n");
		return BAD_NUMBER_OF_ARGUMENTS;
	}

	port = atoi(argv[3]);
	if(port == 0){
		printf("Bad port\n");
		return INCORRECT_ARGUMENT;
	}

	if(pipe(descProc) != 0){
		printf("Error of pipe\n");
		return OTHER_ERROR;
	}

	process.map = NULL;
	process.players = NULL;
	process.desc = descProc;
	process.nbPlayers = 0;
	process.player = createPlayer(argv[1]);

	client.process = &process;
	client.ip = argv[2];
	client.port = port;

	pthread_create(&graphic, NULL, launch_graphic, &client);
	pthread_create(&network, NULL, launch_network, &client);

	pthread_join(graphic, NULL);
	pthread_join(network, NULL);

	return NO_ERROR;
}
