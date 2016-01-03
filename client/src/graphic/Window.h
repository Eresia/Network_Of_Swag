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

typedef struct ScreenInfo ScreenInfo;
struct ScreenInfo{
	SDL_Window* win;
	Process* process;
	int* selectedItem;
	bool* isClosed;
};

void* launch_graphic(void*);
void* displayOnScreen(void*);

#endif
