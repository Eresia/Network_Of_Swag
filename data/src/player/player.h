#ifndef PLAYER
#define PLAYER

#include "../map/Map.h"
#define MAX_PER_CASE 64
#define INV_SIZE 20
#define PIX_PER_BLOCK 24

typedef struct invCase invCase;
struct invCase {
	block desc;
	int number;
};

typedef struct player player;
struct player {
	char *name;
	int position[2];
	// 0 <= position[0] < 2880 -> correspond à x
	// 0 <= position[1] < 2400 -> correspond à y
	invCase inventory[INV_SIZE];
};

player createPlayer(char *playerName);
bool addBlockToInv(player *player, block block);
bool removeBlockFromInv(player *player, int index);
int caseNotFull(player player, block block);
int *inInventory(player player, block block);

#endif
