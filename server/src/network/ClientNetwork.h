#ifndef CLIENT_NETWORK
#define CLIENT_NETWORK

#include "../../../data/src/usefull/Usefull.h"
#include "../../../data/src/player/Player.h"
#include "../game/Protocole.h"
#include "DefineNetwork.h"


typedef struct ClientNetwork ClientNetwork;
struct ClientNetwork{
	SOCKET socket;
	char* name;
	bool isClosed;
	SOCKADDR_IN* info;
	pthread_t *thread;
	pthread_mutex_t *closeMutex;
	Map* map;
	ListPlayer* players;
	int nbTry;
	char* chat;
};

void* begin_communication(void*);

#endif
