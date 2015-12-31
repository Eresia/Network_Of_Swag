#include <stdio.h>
#include <stdlib.h>
#include "../../../server/src/game/Server.h"
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

	block sky = {NONE, SKY};
	block cave = {NONE, CAVE};
	block dirt = {DIRT, SKY};
	block stone = {STONE, CAVE};
	block wood = {WOOD, SKY};
	block iron = {IRON, CAVE};

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

/*block **getMapFromFile(char* filePath);
void displayMap(block **map);

block **getMapFromFile(char *filePath) {

	char mapBloc;
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

	block air = {AIR, BLUE, false};
	block dirt = {DIRT, GREEN, true};
	block stone = {STONE, GRAY, true};
	block wood = {WOOD, BROWN, true};
	block iron = {IRON, ORANGE, true};

	mapFile = fopen(filePath, "r");

	if(mapFile != NULL) {
		for(y=0 ; y<100 ; y++) {
			for(x=0 ; x<120 ; x++) {
				mapblock = fgetc(mapFile);
				switch(mapBloc) {
					case '0':
						map[x][y] = air;
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
					default:
						map[x][y] = air;
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
			printf("\033[%dm ", map[x][y].col);
		}
		printf("\033[0m\n");
	}
	printf("\033[0m\n");
}
*/
