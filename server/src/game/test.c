#include <stdio.h>
#include <stdlib.h>
#include "map.h"

bloc **getMapFromFile(char* filePath);
void displayMap(bloc **map);

int main(int argc, char const *argv[]) {

	bloc **map = getMapFromFile("static.map");

	displayMap(map);

	return 0;
}

bloc **getMapFromFile(char *filePath) {

	char mapBloc;
	FILE *mapFile;
	bloc **map;
	int x=0, y=0;

	map = malloc(120 * sizeof(bloc *));
	if(map != NULL) {
		for(x=0 ; x<120 ; x++) {
			map[x] = malloc(100 * sizeof(bloc));
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

	bloc air = {AIR, BLUE, false};
	bloc dirt = {DIRT, GREEN, true};
	bloc stone = {STONE, GRAY, true};
	bloc wood = {WOOD, BROWN, true};
	bloc iron = {IRON, ORANGE, true};

	mapFile = fopen(filePath, "r");

	if(mapFile != NULL) {
		for(y=0 ; y<100 ; y++) {
			for(x=0 ; x<120 ; x++) {
				mapBloc = fgetc(mapFile);
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

void displayMap(bloc **map)  {
	int x, y;

	for(y=0 ; y<100 ; y++) {
		for(x=0 ; x<120 ; x++) {
			printf("\033[%dm ", map[x][y].col);
		}
		printf("\033[0m\n");
	}
	printf("\033[0m\n");
}
