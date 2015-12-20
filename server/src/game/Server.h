#ifndef SERVER
#define SERVER

#include "../../../data/src/usefull/Usefull.h"
#include "Gameloop.h"
#include "../network/ServerNetwork.h"

typedef struct Server Server;
struct Server{
	Gameloop gl;
	ServerNetwork sn;
};

#endif
