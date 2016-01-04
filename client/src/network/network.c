#include "network.h"


client_network init_client_network(client_network c, char* serv_addr, int serv_port){
	if( c == NULL){
		c = malloc(sizeof(client_network));
		c->write_buffer_busy =  malloc(sizeof(sem_t));
		c->write_buffer_empty =  malloc(sizeof(sem_t));
		c->write_buffer_full =  malloc(sizeof(sem_t));
		c->read_buffer_busy=  malloc(sizeof(sem_t));
		c->read_buffer_empty =  malloc(sizeof(sem_t));
		c->read_buffer_full =  malloc(sizeof(sem_t));
	}
	if ((c->local_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		fprintf(stderr, "[reseau]Impossible d'ouvrir le socket local \n");
		exit(1);
	}
#ifdef DEBUG
	printf("[reseau]connexion au port %i",ntohl(c->local_socket));
#endif

	c->serv_struct->sin_family = AF_INET;
	c->serv_struct->sin_port = htons(serv_port);
	IN_ADDR tmp_addr;
	if (inet_aton(serv_addr, &tmp_addr) == 0) {
		fprintf(stderr, "[reseau]Parsing de l'addresse serveur incorecte \n");
		exit(1);
	}else{
		 strcpy(c->serv_ip_addr , serv_addr);
		c->serv_struct->sin_addr = tmp_addr;
		c->serv_struct->sin_family = AF_INET;
		c->serv_struct->sin_port = htons(serv_port);
		c->serv_port = serv_port;
	}
	// on ecrase ses buffers et on initialise les buffers
	sem_init(c->read_buffer_empty ,1,1);
	sem_init(c->write_buffer_full ,1,1);
	sem_init(c->write_buffer_busy ,1,1);
	sem_init(c->read_buffer_empty ,1,1);
	sem_init(c->read_buffer_full ,0,1);
	sem_init(c->read_buffer_busy ,0,1);

	memset(c->BUFF_IN,0,sizeof(c->BUFF_IN));
	memset(c->BUFF_OUT,0,sizeof(c->BUFF_OUT));
	// on signale le client comme actif
	c->active_client = 1;
	return c;

}
void client_beat(void* client_struct){
	client_network cn = (client_network)  client_struct;
#ifdef DEBUG
	printf("[reseau]demarrage du thread beat vers %s \n ",cn->serv_ip_addr);
#endif
	while(cn->active_client == 1 ){
#ifdef DEBUG
	printf("[reseau]beat client vers %s  \n",cn->serv_ip_addr);
#endif
		send_to_server(cn,"hello\n",strlen("hello\n"));
		sleep(4);
	}

}
void bufferize_from_server(void* client_struct){

	client_network cn = (client_network)  client_struct;
#ifdef DEBUG
	printf("[reseau]demarrage du thread ecoute de %s \n ",cn->serv_ip_addr);
#endif
	int n;
	while(cn->active_client == 1 ){
	sem_wait(cn->read_buffer_full);
	sem_wait(cn->read_buffer_busy);
	if((n = recv(cn->local_socket, cn->BUFF_IN, sizeof (cn->BUFF_IN - 1), 0)) < 0)
	{
		fprintf(stderr, "[reseau]erreur rcv from serveur \n");
		cn->active_client = 0;
		exit(1);
	}
#ifdef DEBUG
	printf("[reseau]reception de  %i octet de %s \n" , sizeof (cn->BUFF_IN - 1) , cn->serv_ip_addr);
#endif
	sem_wait(cn->read_buffer_busy);
	sem_post(cn->read_buffer_full);
	}

}
void bufferize_to_server(void* client_struct){
	client_network cn = (client_network)  client_struct;
#ifdef DEBUG
	printf("[reseau]demarrage du thread envoie de %s \n ",cn->serv_ip_addr);
#endif
	while(cn->active_client == 1 ){
	sem_wait(cn->write_buffer_full);
	sem_wait(cn->write_buffer_busy);
	  if (sendto(cn->local_socket, cn->BUFF_OUT, sizeof(cn->BUFF_OUT) , 0 ,  (SOCKADDR*)cn->serv_struct, sizeof(cn->serv_struct))==-1)
	{
		fprintf(stderr, "erreur rcv from serveur \n");
		cn->active_client = 0;
		exit(1);
	}
#ifdef DEBUG
	printf("[reseau]envoie de  %i octet du buffer vers %s \n" , sizeof(cn->BUFF_OUT)  , cn->serv_ip_addr);
#endif

	sem_post(cn->write_buffer_busy);
	sem_post(cn->write_buffer_empty);
	}

}

int send_to_server(client_network client_struct, void* data , int size){
	char* data_to_send = (char*) data;

	client_network cn = (client_network)  client_struct;

		sem_wait(cn->write_buffer_empty);
		sem_wait(cn->write_buffer_busy);
		strncpy(cn->BUFF_OUT,data_to_send,size);
#ifdef DEBUG
	printf("[reseau]mise en buffer d'envoie de %i octets vers %s \n" , size , cn->serv_ip_addr);
#endif
		sem_post(cn->write_buffer_busy);
		sem_post(cn->write_buffer_full);
		return size;
}
int receive_from_server(client_network client_struct, void* dest){

	char* data_to_send = (char*) dest;

	client_network cn = (client_network)  client_struct;
		int size = sizeof(cn->BUFF_IN);
		sem_wait(cn->write_buffer_empty);
		sem_wait(cn->write_buffer_busy);
		strncpy(data_to_send,cn->BUFF_OUT,size);
		sem_post(cn->write_buffer_busy);
		sem_post(cn->write_buffer_full);
#ifdef DEBUG
	printf("[reseau]recuperation en buffer %i octet de %s \n" , size , cn->serv_ip_addr);
#endif
		return size;
}

void launch_network(void*  client_struct){
	client_network cn = (client_network)  client_struct;
	pthread_create(cn->client_beat_thread, NULL , client_beat, (void*) cn);
	pthread_create(cn->writer_thread, NULL , bufferize_to_server, (void*) cn);
	pthread_create(cn->recever_thread, NULL , bufferize_from_server, (void*) cn);
#ifdef DEBUG
	printf("[reseau]lancement des threads\n");
#endif
	pthread_join(cn->client_beat_thread);
	pthread_join(cn->writer_thread);
	pthread_join(cn->recever_thread);
	free(cn);

}
