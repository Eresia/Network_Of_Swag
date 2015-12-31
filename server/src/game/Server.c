#include <stdio.h>
#include <stdlib.h>
#include "Server.h"

#define MAX_LINE_SIZE 2000

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

void getFileFromMap(block **map, char *filePath) {

	FILE *mapFile;
	char mapBlock;
	mapFile = fopen(filePath, "w+");
	int x=0, y=0;

	if(mapFile != NULL) {
		for(y=0 ; y<100 ; y++) {
			for(x=0 ; x<120 ; x++) {
				//mapBlock = fgetc(mapFile);
				switch(map[x][y].type) {
					case NONE:
						switch(map[x][y].back) {
							case SKY:
								mapBlock = '0';
								break;
							case CAVE:
								mapBlock = '5';
								break;
							default:
								mapBlock = '0';
								break;
						}
						break;
					case DIRT:
						mapBlock = '1';
						break;
					case STONE:
						mapBlock = '2';
						break;
					case WOOD:
						mapBlock = '3';
						break;
					case IRON:
						mapBlock = '4';
						break;
					default:
						mapBlock = '0';
						break;
				}
				fputc(mapBlock, mapFile);
			}
			fputc('\n', mapFile);
		}

		fclose(mapFile);
	}
}

player getPlayerFromFile(char *name, char *filePath) {
	player p;

	FILE *file;
	char playerLine[MAX_LINE_SIZE] = "";
	bool playerFound = false;

	char *nameAccol = malloc(strlen(name)+2);
	strcpy(nameAccol, name);
	strcat(nameAccol, "{");

	file = fopen(filePath, "r");

	if(file != NULL) {
		while(fgets(playerLine, MAX_LINE_SIZE, file) != NULL) {
			if(startsWith(playerLine, nameAccol)) {
				playerFound = true;

				int i=0;
			}
		}

		fclose(file);
	}

	if(! playerFound) {

	}

	return p;
}

void getFileFromPlayers(player *p, int nbPlayers, char *filePath) {

	int i=0;
	FILE *oldFile, *newFile;
	char playerLine[MAX_LINE_SIZE] = "";
	oldFile = fopen(filePath, "r");
	newFile = fopen("server/src/game/temp.players", "a");

	bool playerFound = false;

	char **nameAccol;
	nameAccol = malloc(nbPlayers * sizeof(char*));

	for(i=0 ; i<nbPlayers ; i++) {
		nameAccol[i] = malloc(strlen(p[i].name) * sizeof(char));
		strcpy(nameAccol[i], p[i].name);
		strcat(nameAccol[i], "{");
	}

	if(oldFile != NULL && newFile != NULL) {
		while(fgets(playerLine, MAX_LINE_SIZE, oldFile) != NULL) {
			playerFound = false;
			for(i=0 ; (i<nbPlayers) && (!playerFound); i++) {
				if(startsWith(playerLine, nameAccol[i])) {
					playerFound = true;
					player playerAdd = p[i];
					fprintf(newFile, "%s(%d,%d)(", nameAccol[i], playerAdd.position[0], playerAdd.position[1]);

					int j=0;
					for(j=0 ; j<INV_SIZE ; j++) {
						invCase inv = playerAdd.inventory[j];
						if(inv.number != 0) {
							switch(inv.desc.type) {
								case IRON:
									fprintf(newFile, "IRON");
									break;
								case WOOD:
									fprintf(newFile, "WOOD");
									break;
								case DIRT:
									fprintf(newFile, "DIRT");
									break;
								case STONE:
									fprintf(newFile, "STONE");
									break;
								default:
									fprintf(newFile, "NONE");
									break;
							}
							fprintf(newFile, "-%d", inv.number);
						}
						if(j != INV_SIZE-1) {
							fprintf(newFile, ",");
						}
					}

					fprintf(newFile, ")}\n");
				}
			}
			if(! playerFound) {
				fputs(playerLine, newFile);
			}
		}

		fclose(oldFile);
		fclose(newFile);
	}
}

void displayMap(block **map) {
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
