#include <stdio.h>
#include <stdlib.h>
#include "map.h"

int main(int argc, char const *argv[]) {

	block **map = getMapFromFile("static.map");

	/*
	removeBlock(map, 30, 26);
	removeBlock(map, 31, 26);
	removeBlock(map, 31, 27);

	block iron = {IRON, CAVE, true};

	addBlock(map, 30, 26, iron);
	addBlock(map, 31, 26, iron);
	addBlock(map, 31, 27, iron);
	*/
	displayMap(map);

	return 0;
}

block **getMapFromFile(char *filePath) {

	char mapBlock;
	FILE *mapFile;
	block **map;
	int x=0, y=0;

	map = malloc(120 * sizeof(block *));
	if(map != NULL) {
		for(x=0 ; x<120 ; x++) {
			map[x] = malloc(100 * sizeof(block));
			if(map[x] == NULL) {
				printf("Error loading the map %s", filePath);
				return NULL;
			}
		}
	}
	else {
		printf("Error loading the map %s", filePath);
		return NULL;
	}

	block sky = {NONE, SKY, false};
	block cave = {NONE, CAVE, false};
	block dirt = {DIRT, SKY, true};
	block stone = {STONE, CAVE, true};
	block wood = {WOOD, SKY, true};
	block iron = {IRON, CAVE, true};

	mapFile = fopen(filePath, "r");

	if(mapFile != NULL) {
		for(y=0 ; y<100 ; y++) {
			for(x=0 ; x<120 ; x++) {
				mapBlock = fgetc(mapFile);
				switch(mapBlock) {
					case '0':
						map[x][y] = sky;
						break;
					case '1':
						map[x][y] = dirt;
						break;
					case '2':
						map[x][y] = stone;
						break;
					case '3':
						map[x][y] = wood;
						break;
					case '4':
						map[x][y] = iron;
						break;
					case '5':
						map[x][y] = cave;
						break;
					default:
						map[x][y] = sky;
						break;
				}
			}
			fgetc(mapFile);
		}

		fclose(mapFile);
	}

	return map;
}

void displayMap(block **map)  {
	int x, y;

	for(y=0 ; y<100 ; y++) {
		for(x=0 ; x<120 ; x++) {
			if(map[x][y].type == NONE) {
				printf("\033[%dm ", map[x][y].back);
			}
			else {
				printf("\033[%dm ", map[x][y].type);
			}
		}
		printf("\033[0m\n");
	}
	printf("\033[0m\n");
}
