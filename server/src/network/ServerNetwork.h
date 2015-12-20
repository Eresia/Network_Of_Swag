#ifndef SERVER_NETWORK
#define SERVER_NETWORK

#include "../../../data/src/usefull/Usefull.h"
#include "DefineNetwork.h"
#include "ClientNetwork.h"

#define NB_CLIENT_MAX 100

typedef struct ServerVar* Server;
struct ServerVar{
	int port;
	int connectedClient;
	Client** clients;
};

//server-client
int launch_server(int);
/*void* listenClient(void* );
void* speakClient(void* );
void* testClientConnexion(void* );
void* serverIsFull(void* client);*/

//sockets
int begin_listen(SOCKET*, SOCKADDR_IN*, int);



#endif
