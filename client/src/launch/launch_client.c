#include "launch_client.h"

int main(int argc, char** argv){

	pthread_t graphic, network;
	Process* process = malloc(sizeof(Process));
	Client client;
	client_network cn = malloc(sizeof(struct client_network_struct));
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

	process->map = createVoidMap();
	process->players = NULL;
	process->nbPlayers = 0;
	process->player = createPlayer(argv[1]);

	cn = init_client_network(argv[2], port);
client.process = process;
	client.cn = cn;
	client.ip = argv[2];
	client.port = port;
	client.isClosed = false;
	cn->isClosed = &client.isClosed;



	pthread_create(&graphic, NULL, launch_graphic, &client);
	pthread_create(&network, NULL, launch_network, &client);

	pthread_join(graphic, NULL);
	pthread_join(network, NULL);

	return NO_ERROR;
}
