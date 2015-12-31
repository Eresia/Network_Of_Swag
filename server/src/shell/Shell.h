#ifndef SHELL
#define SHELL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <semaphore.h>

#include "../../../data/src/usefull/Usefull.h"
#include "../game/Gameloop.h"
#include "../game/Server.h"

void* launch_shell(void*);

char* getString();

#endif
