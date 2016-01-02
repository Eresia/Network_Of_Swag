#include "ClientNetwork.h"

void* begin_communication(void* client_void){

	ClientNetwork* cn = (ClientNetwork*) client_void;
	int size;
	bool isClosed;
	char* buff;

	size = sizeof(*cn->info);

	do{

		if(cn->player != NULL){
			buff = Requete_Maj(cn->player, cn->map);

			if(sendto(cn->socket, buff, strlen(buff), 0, (SOCKADDR *)cn->info, (socklen_t) size) < 0)
			{
				printf("Message not send\n");
			}
			else{
				printf("Message send : %s\n", buff);
			}
		}
		else{
			#ifdef DEBUG
			printf("Player is NULL\n");
			#endif
		}

		pthread_mutex_lock(cn->closeMutex);
		isClosed = cn->isClosed;
		pthread_mutex_unlock(cn->closeMutex);
		usleep(25);

	}while(!isClosed);

	#ifdef DEBUG
	printf("Close client\n");
	#endif

	pthread_exit(NULL);
}
