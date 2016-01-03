#include <stdio.h>
#include <stdlib.h>
#include "Map.h"
#include "../usefull/Usefull.h"

block** createVoidMap(){
	block** map = malloc(SIZE_MAX_X*sizeof(block*));
	int i, j;
	for(i = 0; i < SIZE_MAX_X; i++){
		map[i] = malloc(SIZE_MAX_Y*sizeof(block));
		for(j = 0; j < SIZE_MAX_Y; j++){
			block b;
			b.type = VOID;
			b.back = SKY;
			map[i][j] = b;
		}
	}
	return map;
}

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

bool isBlockFree(block **map, int x, int y) {
	if(map[x][y].type == NONE && map[x][y+1].type == NONE) {
		return true;
	}
	return false;
}

char *getBlockName(BlockType type) {
	char *name = malloc(20*sizeof(char));

	switch(type) {
		case NONE:
			sprintf(name, "NONE");
			break;
		case IRON:
			sprintf(name, "IRON");
			break;
		case DIRT:
			sprintf(name, "DIRT");
			break;
		case WOOD:
			sprintf(name, "WOOD");
			break;
		case STONE:
			sprintf(name, "STONE");
			break;
		default:
			sprintf(name, "NONE");
			break;
	}

	return name;
}

BlockType getBlockType(char *name) {
	BlockType type;

	if(strcmp(name, "NONE") == 0) {
		type = NONE;
	}
	else if(strcmp(name, "IRON") == 0) {
		type = IRON;
	}
	else if(strcmp(name, "DIRT") == 0) {
		type = DIRT;
	}
	else if(strcmp(name, "WOOD") == 0) {
		type = WOOD;
	}
	else if(strcmp(name, "STONE") == 0) {
		type = STONE;
	}
	else {
		type = NONE;
	}

	return type;
}

void freeMap(block** map, int size_x, int size_y){
	int i,j;
	for(i = 0; i < size_x; i++){
		for(j = 0; j < size_y; j++){
			free(&(map[i][j].type));
			free(&(map[i][j].back));
			free(&map[i][j]);
		}
	}
}
