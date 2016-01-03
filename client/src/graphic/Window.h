#ifndef WINDOW
#define WINDOW

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <pthread.h>

#include "../../../data/src/usefull/Usefull.h"
#include "../launch/Client.h"
#include "Graphic.h"
#include "../game/Process.h"

void* launch_graphic(void *);

#endif
