#ifndef CLIENT
#define CLIENT

#include "../game/Process.h"
#include "../network/network.h"

typedef struct Client Client;
struct Client{
	Process* process;
	client_network cn;
	bool isClosed;
};

#endif
