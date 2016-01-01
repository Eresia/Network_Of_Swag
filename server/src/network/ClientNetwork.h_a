#ifndef CLIENT_NETWORK
#define CLIENT_NETWORK

#include "../../../data/src/usefull/Usefull.h"
#include "DefineNetwork.h"


typedef struct ClientNetwork ClientNetwork;
struct ClientNetwork{
	SOCKET socket_tcp;
	SOCKET socket_udp;
	int id;
	bool isClosed;
	SOCKADDR_IN* info;
	pthread_t *thread;
	pthread_mutex_t *closeMutex;
};

//#include "CheckClient.h"

void* begin_communication(void*);
void* speak_to_client(void*);

#endif
