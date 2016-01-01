#ifndef CHECKCLIENT
#define CHECKCLIENT

//General includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//Threads
#include <pthread.h>

#include "ListClient.h"
#include "ClientNetwork.h"

typedef struct CheckClient CheckClient;
struct CheckClient{
	ClientNetwork* client;
	ListClient* list;
};

void* checkIfClientIsConnected(void*);

#endif
