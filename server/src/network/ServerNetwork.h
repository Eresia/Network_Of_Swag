#ifndef SERVER_NETWORK
#define SERVER_NETWORK

#include "../../../data/src/usefull/Usefull.h"
#include "DefineNetwork.h"
#include "ClientNetwork.h"

#define NB_CLIENT_MAX 100

typedef struct ServerNetwork ServerNetwork;
struct ServerNetwork{
	int port;
	int connectedClient;
	ClientNetwork** clients;
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



#endif
