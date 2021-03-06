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

typedef struct Gameloop Gameloop;
struct Gameloop{
	bool isStopped;
	pthread_t* thread;
	pthread_mutex_t* stopMutex;
	pthread_mutex_t* mapMutex;
	pthread_mutex_t* playerMutex;
	int* desc;
	ListPlayer* listPlayer;
	Map* map;
};

void* launch_gameloop(void*);
char* waitMessage(int, int, int);

#endif
