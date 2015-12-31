#ifndef SERVER_NETWORK
#define SERVER_NETWORK

#include "../../../data/src/usefull/Usefull.h"
#include "DefineNetwork.h"
#include "ClientNetwork.h"
#include "ListClient.h"

#define NB_CLIENT_MAX 100

#define TIMEOUT -5

typedef struct ServerNetwork ServerNetwork;
struct ServerNetwork{
	int port;
	int connectedClient;
	ListClient clients;
	pthread_t* thread;
};

#include "../game/Server.h"

//server-client
void* launch_network(void*);
/*void* listenClient(void* );
void* speakClient(void* );
void* testClientConnexion(void* );
void* serverIsFull(void* client);*/

//sockets
int begin_listen(SOCKET*, int);
SOCKET waitConnexion(SOCKET, SOCKADDR_IN*, int, int);



#endif
