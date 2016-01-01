#ifndef SERVER
#define SERVER

#include "../../../data/src/usefull/Usefull.h"
#include "Gameloop.h"
#include "../network/ServerNetwork.h"
#include "../../../data/src/map/Map.h"
#include "../../../data/src/player/player.h"

typedef struct Server Server;
struct Server{
	Gameloop gl;
	ServerNetwork sn;
};

block **getMapFromFile(char *filePath);
void getFileFromMap(block **map, char *filePath);
player loadPlayer(char *name);
bool savePlayer(player p);
bool savePlayers(player *p, int nbPlayers);

/* Les 2 fonctions peuvent être utiles pour le jeu final, mais pas sûr */
void displayMap(block **map);
void displayMapPlayer(block **map, int player[]);

#endif
