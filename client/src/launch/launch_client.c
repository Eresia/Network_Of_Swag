#include "launch_client.h"

int main(int argc, char** argv){

	pthread_t graphic, network;
	Process process;
	Client client;
	int* descProc = malloc(2*sizeof(int));

	if(argc != 2){
		printf("Not expected arguments\n");
		return BAD_NUMBER_OF_ARGUMENTS;
	}

	if(pipe(descProc) != 0){
		printf("Error of pipe\n");
		return OTHER_ERROR;
	}

	process.map = NULL;
	process.players = NULL;
	process.desc = descProc;
	process.player = createPlayer(argv[1]);

	client.process = &process;

	pthread_create(&graphic, NULL, launch_graphic, &client);
	pthread_create(&network, NULL, launch_network, &client);

	pthread_join(graphic, NULL);
	pthread_join(network, NULL);

	return NO_ERROR;
}
