#ifndef SERVER
#define SERVER

#include "../../../data/src/usefull/Usefull.h"
#include "Gameloop.h"
#include "../network/ServerNetwork.h"
#include "../../../data/src/map/Map.h"
#include "../../../data/src/player/Player.h"

typedef struct Server Server;
struct Server{
	Gameloop gl;
	ServerNetwork sn;
};

Map* getMapFromFile(char *name);
void getFileFromMap(Map* map);
Player* loadPlayer(char *name);
bool savePlayer(Player p);
bool savePlayers(Player *p, int nbPlayers);

/* Les 2 fonctions peuvent être utiles pour le jeu final, mais pas sûr */
void displayMap(block **map);
void displayMapPlayer(block **map, int player[]);

#endif
