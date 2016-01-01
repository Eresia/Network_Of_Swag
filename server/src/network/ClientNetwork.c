#include "ClientNetwork.h"
#include "CheckClient.h"

void* begin_communication(void* client_void){

	CheckClient* client = (CheckClient*) client_void;
	ClientNetwork* cn = client->client;
	char* buf = calloc(1501, sizeof(char));
	int size;
	bool isClosed;

	size = sizeof(*cn->info);

	do{

		strcpy(buf, "test\n");

		if(sendto(cn->socket, buf, strlen(buf), 0, (SOCKADDR *)cn->info, (socklen_t) size) < 0)
		{
			printf("Message not send\n");
		}
		else{
			printf("Message send\n");
		}

		pthread_mutex_lock(cn->closeMutex);
		isClosed = cn->isClosed;
		pthread_mutex_unlock(cn->closeMutex);

		sleep(3);

	}while(!isClosed);

	pthread_exit(NULL);
}
