#include "CheckClient.h"

void* checkIfClientIsConnected(void* clients_void){

	ListClient* clients = (ListClient*) clients_void;

	while(true){
		ItemList client = clients->firstItem;
		while(client != NULL){
			ClientNetwork* cn = client->client;
			cn->nbTry++;
			if(cn->nbTry == NB_MAX_TRY){
				printf("Client %s disconnected\n", cn->name);
				client = client->next;
				removeClient(clients, cn);
			}
			else{
				#ifdef DEBUG
				//printf("nbTry : %d\n", cn->nbTry);
				#endif
				client = client->next;
			}
		}
		sleep(1);
	}

	pthread_exit(NULL);
}
