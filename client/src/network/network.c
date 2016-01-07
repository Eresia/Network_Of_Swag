#include "network.h"
#include "../launch/launch_client.h"

client_network init_client_network(char* serv_addr, int serv_port){
	client_network c;
	struct hostent *hostinfo;
	hostinfo = NULL;
	printf("port : %d\n", serv_port);

	c = malloc(sizeof(struct client_network_struct));

	if ((c->local_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		fprintf(stderr, "[reseau]Impossible d'ouvrir le socket local \n");
		pthread_exit(NULL);
	}

	#ifdef DEBUG
	printf("[reseau]connexion au port %i\n", c->local_socket);
	#endif

	hostinfo = gethostbyname(serv_addr);
	if (hostinfo == NULL)
	{
	    fprintf (stderr, "Unknown host %s.\n", serv_addr);
	    exit(1);
	}

	c->serv_struct = malloc(sizeof(SOCKADDR_IN));
	c->serv_struct->sin_family = AF_INET;
	c->serv_struct->sin_port = htons(serv_port);
	c->serv_struct->sin_addr = *(IN_ADDR *) hostinfo->h_addr;
	c->serv_port = serv_port;

	c->BUFF_IN = calloc(SIZE_MESSAGE_MAX+4, sizeof(char));
	c->BUFF_OUT = calloc(SIZE_MESSAGE_MAX, sizeof(char));
	return c;

}
void* client_beat(void* client_struct){
	client_network cn;
	cn = (client_network)  client_struct;
	#ifdef DEBUG
	printf("[reseau]demarrage du thread beat vers %s \n ",cn->serv_ip_addr);
	#endif
	while(*cn->isClosed == false){
		/*#ifdef DEBUG
		printf("[reseau]beat client vers %s  \n",cn->serv_ip_addr);
		#endif*/
		send_to_server(cn,"hello\0",strlen("hello"));
		sleep(1);
	}
	pthread_exit(NULL);
}

void* bufferize_from_server(void* client_struct){

	Client* c;
	client_network cn;
	int n, size;
	fd_set rfds;
	struct timeval tv;

	c = (Client*) client_struct;
	cn = c->cn;

	#ifdef DEBUG
	printf("[reseau]demarrage du thread ecoute de %s \n ",cn->serv_ip_addr);
	#endif

	while(*cn->isClosed == false){
		int iResult;
		iResult = 0;
		size = sizeof(*cn->serv_struct);

		FD_ZERO(&rfds);
		FD_SET(cn->local_socket, &rfds);

		tv.tv_sec = (long) 2;
		tv.tv_usec = (long) 0;

		iResult = select(cn->local_socket + 1, &rfds, (fd_set *) 0, (fd_set *) 0, &tv);
		if(iResult > 0)
		{
			if((n = recvfrom(cn->local_socket, cn->BUFF_IN, SIZE_MESSAGE_MAX+4, 0, (SOCKADDR*)cn->serv_struct, (socklen_t*) &size)) < 0)
			{
				fprintf(stderr, "[reseau]erreur rcv from serveur \n");
				*cn->isClosed = true;
				pthread_exit(NULL);
			}
			else{
				char* buff;

				cn->BUFF_IN[n] = '\0';
				buff = calloc(SIZE_MESSAGE_MAX+4, sizeof(char));
				strcpy(buff, cn->BUFF_IN);
				if(strcmp(strtok(buff, ","), "-2") == 0){
					int nbParts;
					nbParts = atoi(strtok(NULL, ""));
					if(nbParts != 0){
						bool isOk;
						int total, i;

						isOk = true;
						total = 0;
						buff = calloc(SIZE_MESSAGE_MAX*nbParts+1, sizeof(char));
						for(i = 0; i < nbParts; i++){
							memset(cn->BUFF_IN, 0, SIZE_MESSAGE_MAX);
							if((n = recvfrom(cn->local_socket, cn->BUFF_IN, SIZE_MESSAGE_MAX+3, 0, (SOCKADDR*)cn->serv_struct, (socklen_t*) &size)) < 0)
							{
								isOk = false;
								break;
							}
							else{
								if(strcmp(strtok(cn->BUFF_IN, ","), "-3") == 0){
									char* r;
									r = strtok(NULL, "");
									if(r != NULL){
										strcat(buff, r);
										total += n-3;
									}
									else{
										isOk = false;
										break;
									}
								}
								else{
									isOk = false;
									break;
								}
							}
						}
						if(isOk){
							buff[total] = '\0';
							parse_Protocole(c->process, buff);
						}
					}
				}
				else{
					parse_Protocole(c->process, cn->BUFF_IN);
				}
				/*#ifdef DEBUG
				printf("[reseau]reception de %i octet \n"de %s \n" , (int) n , cn->BUFF_IN);
				#endif*/
				memset(cn->BUFF_IN, 0, SIZE_MESSAGE_MAX+4);
			}
		}
		else
		{
			printf("Server closed\n");
			*cn->isClosed = true;
			return NULL;
		}
	}
	pthread_exit(NULL);
}

void* bufferize_to_server(void* client_struct){
	client_network cn;
	cn = (client_network)  client_struct;
	#ifdef DEBUG
	printf("[reseau]demarrage du thread envoie de %s \n ",cn->serv_ip_addr);
	#endif
	while(*cn->isClosed == false){
		if(strlen(cn->BUFF_OUT) > 0){
			if (sendto(cn->local_socket, cn->BUFF_OUT, (int) strlen(cn->BUFF_OUT) , 0 ,  (SOCKADDR*)cn->serv_struct, sizeof(*cn->serv_struct))==-1)
			{

				perror("erreur d'envoi ");
				fprintf(stderr, "erreur send from serveur %s \n", cn->BUFF_OUT);
				*cn->isClosed = true;
				pthread_exit(NULL);
			}
			/*#ifdef DEBUG
			printf("[reseau]envoie de  %i octet du buffer vers %s \n" , (int) strlen(cn->BUFF_OUT)  , cn->serv_ip_addr);
			#endif*/
			memset(cn->BUFF_OUT, 0, SIZE_MESSAGE_MAX);
		}
		usleep(25000);
	}
	pthread_exit(NULL);
}

int send_to_server(client_network client_struct, void* data , int size){
	char* data_to_send;
	client_network cn;

	data_to_send = (char*) data;
	cn = (client_network)  client_struct;

	strncpy(cn->BUFF_OUT,data_to_send,size);
	/*#ifdef DEBUG
	printf("[reseau]mise en buffer d'envoie de %i octets vers %s \n" , size , cn->serv_ip_addr);
	#endif*/
	return size;
}
int receive_from_server(client_network client_struct, void* dest){

	char* data_to_send;

	client_network cn;
	int size;

	cn = (client_network)  client_struct;
	data_to_send = (char*) dest;
	size = strlen(cn->BUFF_IN);

	if(size != 0){
		strncpy(data_to_send,cn->BUFF_IN,size);
		memset(cn->BUFF_IN, 0, SIZE_MESSAGE_MAX+4);
	}

	/*#ifdef DEBUG
	printf("[reseau]recuperation en buffer %i octet de %s \n" , size , cn->serv_ip_addr);
	#endif*/
	return size;
}

void* processingReadData(void* client_struct){
	Client* c;
	client_network cn;
	char* buff;

	c = (Client*) client_struct;
	cn = c->cn;
	#ifdef DEBUG
	printf("[reseau]demarrage du thread traitement de donnes de %s \n ",cn->serv_ip_addr);
	#endif
	buff = calloc(SIZE_MESSAGE_MAX + 1, sizeof(char));
	while(*cn->isClosed == false){
		if(receive_from_server(cn, buff) != 0){
			if(strlen(buff) > 0){

			}
		}
		sleep(1);
	}

	pthread_exit(NULL);
}

void* launch_network(void* client_struct){
	Client* c;
	client_network cn;
	char* buff;
	pthread_t beatThread, writerThread, receverThread;

	c = (Client*) client_struct;
	cn = c->cn;
	buff = calloc(4+strlen(c->process->player->name)+1, sizeof(char));
	sprintf(buff, "new,%s", c->process->player->name);

	pthread_create(&beatThread, NULL , client_beat, cn);
	pthread_create(&writerThread, NULL , bufferize_to_server, cn);
	pthread_create(&receverThread, NULL , bufferize_from_server, c);
	#ifdef DEBUG
	printf("[reseau]lancement des threads\n");
	#endif

	sleep(1);
	send_to_server(cn, buff, strlen(buff));

	pthread_join(beatThread, NULL);
	pthread_join(writerThread, NULL);
	pthread_join(receverThread, NULL);
	free(cn);
	closesocket(cn->local_socket);
	#ifdef DEBUG
	printf("Close network\n");
	#endif
	pthread_exit(NULL);
}
