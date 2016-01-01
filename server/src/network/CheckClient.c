#include "CheckClient.h"

void* checkIfClientIsConnected(void* client_void){

	CheckClient* client = (CheckClient*) client_void;
	char* buf = malloc(sizeof(char));
	
	recv(client->client->socket_tcp, buf, 1, 0);

	#ifdef DEBUG
		printf("Client was deconnected\n");
	#endif

	removeClient(client->list, client->client);

	pthread_mutex_lock(client->client->closeMutex);
	client->client->isClosed = true;
	pthread_mutex_unlock(client->client->closeMutex);

	pthread_exit(NULL);
}
