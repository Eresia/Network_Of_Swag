#ifndef CHECKCLIENT
#define CHECKCLIENT

/*General includes*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*Threads*/
#include <pthread.h>

#include "ListClient.h"

#define NB_MAX_TRY 5

typedef struct CheckClient CheckClient;
struct CheckClient{
	ListClient* clients;
	Map* map;
};

void* checkIfClientIsConnected(void*);

#endif
