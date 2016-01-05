#include "ClientNetwork.h"
#include "CheckClient.h"

void* begin_communication(void* client_void){

	printf("In The thread\n");

	ClientNetwork* cn = (ClientNetwork*) client_void;
	int size;
	bool isClosed, isInList;
	char* buff;
	int i, j, n, nbParts;
	char** parts;

	size = sizeof(*cn->info);
	isInList = isPlayerInListByName(cn->players, cn->name);

	do{
		if(isInList){
			buff = Requete_Maj(cn->name, cn->players, cn->map);

			if(buff != NULL){

				nbParts = ((strlen(buff)/SIZE_MESSAGE_MAX) + ((strlen(buff)%SIZE_MESSAGE_MAX) > 0) + 1);
				parts = malloc(nbParts*sizeof(char*));
				parts[0] = malloc(20*sizeof(char));
				sprintf(parts[0], "-2,%d", nbParts - 1);
				for(i = 1; i < nbParts; i++){
					parts[i] = calloc(SIZE_MESSAGE_MAX + 4, sizeof(char));
					strcat(parts[i], "-3,");
					for(j = 0; (j < SIZE_MESSAGE_MAX) && (buff[i*SIZE_MESSAGE_MAX] != '\0'); j++){
						parts[i][j+3] = buff[(i - 1)*SIZE_MESSAGE_MAX + j];
					}
					parts[i][j+3] = '\0';
				}

				for(i = 0; i < nbParts; i++){
					if((n = sendto(cn->socket, parts[i], strlen(parts[i]), 0, (SOCKADDR *)cn->info, (socklen_t) size)) < 0)
					{
						#ifdef DEBUG
						printf("Message not send\n");
						#endif
					}
					else{
						#ifdef DEBUG
						//printf("Size send : %d\n", n);
						//printf("Message send : %s\n", parts[i]);
						#endif
					}
				}

			}
			else{
				#ifdef DEBUG
				printf("Player is NULL\n");
				#endif
			}

			if(strlen(cn->chat) != 0){
				if((n = sendto(cn->socket, cn->chat, strlen(cn->chat), 0, (SOCKADDR *)cn->info, (socklen_t) size)) < 0)
				{
					#ifdef DEBUG
					printf("Message not send\n");
					#endif
				}
				else{
					#ifdef DEBUG
					//printf("Size send : %d\n", n);
					//printf("Message send : %s\n", parts[i]);
					#endif
				}
				memset(cn->chat, 0, SIZE_MESSAGE_MAX);
			}
		}
		else{
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
