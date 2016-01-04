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
				pthread_mutex_lock(cn->closeMutex);
				removeClient(clients, cn);
				removePlayerByName(cn->players, cn->name);
				pthread_mutex_unlock(cn->closeMutex);
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
