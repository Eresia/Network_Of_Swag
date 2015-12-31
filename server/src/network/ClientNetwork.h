#ifndef CLIENT_NETWORK
#define CLIENT_NETWORK

#include "../../../data/src/usefull/Usefull.h"
#include "DefineNetwork.h"


typedef struct ClientNetwork ClientNetwork;
struct ClientNetwork{
	SOCKET socket;
	int id;
	bool isClosed;
	SOCKADDR_IN* info;
	pthread_t *thread;
	pthread_mutex_t *closeMutex;
};


#endif
