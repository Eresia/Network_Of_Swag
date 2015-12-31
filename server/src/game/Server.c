#include <stdio.h>
#include <stdlib.h>
#include "map.h"

/*int main(int argc, char const *argv[]) {

	int player[2] = {68, 50};
	block **map = getMapFromFile("static.map");

	while(map[player[0]][player[1]+1].type == NONE) {
		player[1] ++;
		displayMapPlayer(map, player);
	}

	return 0;
}*/

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

void displayMapPlayer(block **map, int player[]) {
	int startX = 0, startY = 0, x, y;

	if(player[0] > 90) {
		startX = 60;
	}
	else if(player[0] > 30) {
		startX = player[0] - 30;
	}

	if(player[1] > 90) {
		startY = 80;
	}
	else if(player[1] > 10) {
		startY = player[1] - 10;
	}

	system("clear");

	for(y=startY ; y<startY+20 ; y++) {
		for(x=startX ; x<startX+60 ; x++) {
			if(map[x][y].type == NONE) {
				printf("\033[%dm", map[x][y].back);
			}
			else {
				printf("\033[%dm", map[x][y].type);
			}

			if(x == player[0]) {
				if(y == player[1] || y+1 == player[1]) {
					printf("\033[37mX");
				}
				else {
					printf(" ");
				}
			}
			else {
				printf(" ");
			}
		}
		printf("\033[0m\n");
	}
	printf("\033[0m\n");

}
