#ifndef CLIENT_NETWORK
#define CLIENT_NETWORK

#include "../../../data/src/usefull/Usefull.h"
#include "DefineNetwork.h"


typedef struct Client Client;
struct Client{
	SOCKET id;
	bool isClosed;
	char *ip;
	pthread_mutex_t *mutexClose;
};


#endif
