#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#define HEIGHT 1000 //Taille de la fenêtre
#define WIDTH 1000

#define XMAXMAP 2880 //Limite de la carte
#define YMAXMAP 2400

typedef enum {AIR, DIRT, STONE, WOOD, IRON} blocType;
typedef enum {BLUE=44, GREEN=42, GRAY=47, BROWN=43, ORANGE=41} Color;
typedef enum {false, true} Boolean;

typedef struct {
	blocType type;
	Color col;
	Boolean solid;
} bloc;

/*
0:air
1:dirt
2:stone
3:wood
4:iron
*/

void printBlock(SDL_Window* window, bloc blocValue, int x, int y);
void printMap(SDL_Window* window,bloc **map, int x, int y);

Boolean waitEvent(SDL_Event event, SDL_Window* window);

void loadBmp();
void freeBmp();
