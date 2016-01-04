#include "ClientNetwork.h"
#include "CheckClient.h"

void* begin_communication(void* client_void){

	ClientNetwork* cn = (ClientNetwork*) client_void;
	int size;
	bool isClosed, isInList;
	char* buff;
	int n;

	size = sizeof(*cn->info);
	isInList = isPlayerInListByName(cn->players, cn->name);

	do{

		if(isInList){
			buff = Requete_Maj(cn->name, cn->players, cn->map);

			if((n = sendto(cn->socket, buff, strlen(buff), 0, (SOCKADDR *)cn->info, (socklen_t) size)) < 0)
			{
				#ifdef DEBUG
				printf("Message not send\n");
				#endif
			}
			else{
				#ifdef DEBUG
				printf("Size send : %d\n", n);
				//printf("Message send : %s\n Size : %d\n", buff, (int) strlen(buff));
				#endif
			}
		}
		else{
			#ifdef DEBUG
			printf("Player is NULL\n");
			#endif
			isInList = isPlayerInListByName(cn->players, cn->name);
		}

		pthread_mutex_lock(cn->closeMutex);
		isClosed = cn->isClosed;
		pthread_mutex_unlock(cn->closeMutex);
		usleep(25000);

	}while(!isClosed);

	#ifdef DEBUG
	printf("Close client\n");
	#endif

	pthread_exit(NULL);
}
