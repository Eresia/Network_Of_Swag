#include <stdio.h>
#include <stdlib.h>
#include "map.h"

block **removeBlock(block **map, int x, int y) {
	if(map[x][y].type != NONE) {
		map[x][y].type = NONE;
	}

	return map;
}

block **addBlock(block **map, int x, int y, block newBlock) {
	if(map[x][y].type == NONE) {
		map[x][y] = newBlock;
	}

	return map;
}

Boolean isBlockFree(block **map, int x, int y) {
	if(map[x][y].type == NONE && map[x][y+1].type == NONE) {
		return true;
	}
	return false;
}
