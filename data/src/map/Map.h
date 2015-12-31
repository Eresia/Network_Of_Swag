#ifndef MAP
#define MAP

#include "../usefull/Usefull.h"
/* Les valeurs des enum BlockType et Background sont amenée à être supprimées par la suite */
typedef enum {NONE=0, IRON=41, WOOD=43, DIRT=42, STONE=47} BlockType;
typedef enum {CAVE=40, SKY=44} Background;
// typedef enum {false, true} Boolean;

typedef struct {
	BlockType type;
	Background back;
	bool isSolid;
} block;

block **removeBlock(block **map, int x, int y);
block **addBlock(block **map, int x, int y, block newBlock);
bool isBlockFree(block **map, int x, int y);

#endif
