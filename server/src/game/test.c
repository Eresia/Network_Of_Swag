#include <stdio.h>
#include <stdlib.h>
#include "map.h"

void displayMap(bloc map[120][100]);

int main(int argc, char const *argv[]) {

	char mapBloc;
	FILE *mapFile = NULL;
	bloc map[120][100];
	int x=0, y=0;

	bloc air = {AIR, BLUE, false};
	bloc dirt = {DIRT, GREEN, true};
	bloc stone = {STONE, GRAY, true};
	bloc wood = {WOOD, BROWN, true};
	bloc iron = {IRON, ORANGE, true};

	mapFile = fopen("static.map", "r");

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

	displayMap(map);

	return 0;
}

void displayMap(bloc map[120][100])  {
	int x, y;

	for(y=0 ; y<100 ; y++) {
		for(x=0 ; x<120 ; x++) {
			printf("\033[%dm ", map[x][y].col);
		}
		printf("\033[0m\n");
	}
	printf("\033[0m\n");
}
