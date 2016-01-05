#ifndef SERVER_NETWORK
#define SERVER_NETWORK

#include "../../../data/src/usefull/Usefull.h"
#include "../game/Protocole.h"
#include "DefineNetwork.h"
#include "ClientNetwork.h"
#include "ListClient.h"

#define NB_CLIENT_MAX 100

#define TIMEOUT -5

typedef struct ServerNetwork ServerNetwork;
struct ServerNetwork{
	int port;
	ListClient* clients;
	pthread_t* thread;
	int* desc;
};

//server-client
void* launch_network(void*);
/*void* listenClient(void* );
void* speakClient(void* );
void* testClientConnexion(void* );
void* serverIsFull(void* client);*/

//sockets
int begin_listen(SOCKET*, int);
SOCKADDR_IN* waitConnexion(SOCKET, char*, int, int);



#endif
