#include "ClientNetwork.h"
#include "CheckClient.h"

void* begin_communication(void* client_void){

	CheckClient* client = (CheckClient*) client_void;
	ClientNetwork* cn = client->client;
	char* buf = calloc(1501, sizeof(char));
	int size;
	int n;
	SOCKADDR_IN info;
	bool isClosed;
	pthread_t thread_speak;

	size = sizeof(info);
	if((n = recvfrom(cn->socket_udp, buf, 2, 0, (SOCKADDR *)&info, (socklen_t*) &size)) < 0)
	{
		#ifdef DEBUG
		printf("Client not connected\n");
		#endif
		removeClient(client->list, cn);
		pthread_exit(NULL);
	}

	#ifdef DEBUG
	printf("Client connected\n");
	#endif

	if(pthread_create(&thread_speak, NULL, speak_to_client, client_void) != 0){
		#ifdef DEBUG
		  printf("Threads speak not created\n");
		#endif
		removeClient(client->list, cn);
		pthread_exit(NULL);
	}

	do{

		if((n = recvfrom(cn->socket_udp, buf, 1501, 0, (SOCKADDR *)&info, (socklen_t*) &size)) < 0)
		{
			#ifdef DEBUG
			printf("Message not received\n");
			#endif
		}
		else{
			buf[n] = '\0';
			#ifdef DEBUG
			printf("Message received : %s\n", buf);
			#endif
		}

		/*if(sendto(cn->socket_udp, buf, strlen(buf), 0, (SOCKADDR *)&info, (socklen_t) size) < 0)
		{
			printf("Message not send\n");
		}
		else{
			printf("Message send\n");
		}*/

		pthread_mutex_lock(cn->closeMutex);
		isClosed = cn->isClosed;
		pthread_mutex_unlock(cn->closeMutex);

	}while(!isClosed);


	pthread_join(thread_speak, NULL);
	pthread_exit(NULL);
}

void* speak_to_client(void* client_void){

	#ifdef DEBUG
	printf("Speak Launch\n");
	#endif

	pthread_exit(NULL);
}
