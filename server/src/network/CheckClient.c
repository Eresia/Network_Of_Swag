#include "CheckClient.h"
#include "../game/Server.h"

void* checkIfClientIsConnected(void* clients_void){

	CheckClient* cc = (CheckClient*) clients_void;
	ListClient* clients = cc->clients;

	while(true){
		ItemList client = clients->firstItem;
		while(client != NULL){
			ClientNetwork* cn = client->client;
			cn->nbTry++;
			if(cn->nbTry == NB_MAX_TRY){
				printf("Client %s disconnected\n", cn->name);
				client = client->next;
				savePlayer(*getPlayerByName(cn->players, cn->name));
				removeClient(clients, cn);
				removePlayerByName(cn->players, cn->name);
			}
			else{
				#ifdef DEBUG
				//printf("nbTry : %d\n", cn->nbTry);
				#endif
				client = client->next;
			}
		}
		getFileFromMap(cc->map);
		sleep(1);
	}

	pthread_exit(NULL);
}
