#ifndef CLIENT_NETWORK
#define CLIENT_NETWORK

#include "../../../data/src/usefull/Usefull.h"
#include "../../../data/src/player/player.h"
#include "DefineNetwork.h"


typedef struct ClientNetwork ClientNetwork;
struct ClientNetwork{
	SOCKET socket;
	char* name;
	bool isClosed;
	SOCKADDR_IN* info;
	pthread_t *thread;
	pthread_mutex_t *closeMutex;
};

//#include "CheckClient.h"

void* begin_communication(void*);

#endif
