#ifndef MAP
#define MAP

#include "../usefull/Usefull.h"

#define SIZE_MAX_X 120
#define SIZE_MAX_Y 100

/* Les valeurs des enum BlockType et Background sont amenée à être supprimées par la suite */
typedef enum {NONE=0, VOID=1, IRON=41, WOOD=43, DIRT=42, STONE=47} BlockType;
typedef enum {CAVE=40, SKY=44} Background;
// typedef enum {false, true} Boolean;

typedef struct block block;
struct block{
	BlockType type;
	Background back;
};

typedef struct Map Map;
struct Map{
	block** map;
	int* spawn;
	char* name;
};

block** createVoidMap();
block **removeBlock(block **map, int x, int y);
block **addBlock(block **map, int x, int y, block newBlock);
bool isBlockFree(block **map, int x, int y);
char *getBlockName(BlockType type);
BlockType getBlockType(char *name);

void freeMap(block**, int, int);

#endif
