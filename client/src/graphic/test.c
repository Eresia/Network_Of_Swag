#include <stdio.h>
#include <stdlib.h>
#include "../../../server/src/game/Server.h"


Map* getMapFromFile(char *filePath) {

	char mapBlock;
	FILE *mapFile;
	block **map;
	Map* endMap = malloc(sizeof(Map));
	int x=0, y=0;
	int* spawn = malloc(2*sizeof(int));

	map = malloc(SIZE_MAX_X * sizeof(block *));
	if(map != NULL) {
		for(x=0 ; x<SIZE_MAX_X ; x++) {
			map[x] = malloc(SIZE_MAX_Y * sizeof(block));
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
		for(y=0 ; y<SIZE_MAX_Y ; y++) {
			for(x=0 ; x<SIZE_MAX_X ; x++) {
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
					case '9':
						#ifdef DEBUG
						printf("Spawn : %d, %d\n", x, y);
						#endif
						map[x][y] = sky;
						spawn[0] = x;
						spawn[1] = y;
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
	else{
		printf("Problem with map file\n");
	}

	endMap->map = map;
	endMap->spawn = spawn;
	return endMap;
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
