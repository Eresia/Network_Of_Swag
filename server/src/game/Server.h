#ifndef SERVER
#define SERVER

#include "../../../data/src/usefull/Usefull.h"
#include "Gameloop.h"
#include "../network/ServerNetwork.h"

typedef struct Server Server;
struct Server{
	Gameloop gl;
	ServerNetwork sn;
};

block **getMapFromFile(char *filePath);
void getFileFromMap(block **map, char *filePath);
/* Les 2 fonctions peuvent être utiles pour le jeu final, mais pas sûr */
void displayMap(block **map);
void displayMapPlayer(block **map, int player[]);

#endif
