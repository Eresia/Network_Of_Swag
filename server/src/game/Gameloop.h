#ifndef GAMELOOP
#define GAMELOOP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../../../data/src/usefull/Usefull.h"
#include "Server.h"

typedef struct Gameloop Gameloop;
struct Gameloop{

};

void* launch_gameloop(void*);
void makeCommand(char*);

#endif
