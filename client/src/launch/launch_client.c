#include "launch_client.h"

int main(int argc, char** argv){

	pthread_t graphic, network, shell;
	Process* process = malloc(sizeof(Process));
	Client client;
	client_network cn = malloc(sizeof(struct client_network_struct));
	int port;

	if(argc != 4 && argc != 5){
		printf("Not expected arguments\n");
		return BAD_NUMBER_OF_ARGUMENTS;
	}

	port = atoi(argv[3]);
	if(port == 0){
		printf("Bad port\n");
		return INCORRECT_ARGUMENT;
	}

	process->map = createVoidMap();
	process->player = createPlayer(argv[1]);
	process->nbPlayers = 0;
	process->players = NULL;

	cn = init_client_network(argv[2], port);
	client.process = process;
	client.cn = cn;
	client.isClosed = false;
	cn->isClosed = &client.isClosed;
	if(argc == 5){
		client.logo = false;
	}
	else{
		client.logo = true;
	}


	pthread_create(&graphic, NULL, launch_graphic, &client);
	pthread_create(&network, NULL, launch_network, &client);
	pthread_create(&shell, NULL, launch_shell, &client);

	pthread_join(graphic, NULL);
	pthread_join(network, NULL);

	return NO_ERROR;
}
