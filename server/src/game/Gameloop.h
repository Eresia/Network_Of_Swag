#ifndef GAMELOOP
#define GAMELOOP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <semaphore.h>

#include "../../../data/src/usefull/Usefull.h"
#include "../../../data/src/player/ListPlayer.h"
#include "../../../data/src/map/Map.h"

#define SIZE_MESSAGE_MAX 1500

typedef struct Gameloop Gameloop;
struct Gameloop{
	bool isStopped;
	pthread_t* thread;
	pthread_mutex_t* stopMutex;
	int* desc;
	ListPlayer* listPlayer;
	block** map;
};

void* launch_gameloop(void*);

#endif
