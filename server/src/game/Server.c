#include <stdio.h>
#include <stdlib.h>
#include "Server.h"
#include "../network/ServerNetwork.h"

#define MAX_LINE_SIZE 2000

/* Le main de ce fichier sera enlevé par la suite, il me sert à faire des tests (Vincent) 
int main(int argc, char const *argv[]) {

	block **map = getMapFromFile("server/saves/static.map");
 	map = removeBlock(map, 64, 43);
 	block ironBlock = {IRON, CAVE};
 	map = addBlock(map, 64, 43, ironBlock);
 	getFileFromMap(map, "server/saves/static2.map");
 	map = getMapFromFile("server/saves/static2.map");

	//player p0 = createPlayer("agurato");
	//savePlayer(p0);
    player p1 = loadPlayer("agurato");
    block woodBlock = {WOOD, SKY};
    addBlockToInv(&p1, ironBlock);
    addBlockToInv(&p1, woodBlock);
    removeBlockFromInv(&p1, 0);
    removeBlockFromInv(&p1, 1);
    int i=0;

    printf("%d, %d\n", p1.position[0], p1.position[1]);
    for(i=0 ; i<INV_SIZE ; i++) {
        printf("%s-%d\n", getBlockName(p1.inventory[i].desc.type), p1.inventory[i].number);
    }

    //displayMap(map);

    return 0;
}
*/

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

player loadPlayer(char *name) {
    player p = createPlayer(name);

    FILE *file;
    char filePath[80];
    sprintf(filePath, "server/saves/%s.player", name);
    file = fopen(filePath, "r");

    if(file != NULL) {
        char line[25];
        int i=0;

        for(i=0 ; i<2 ; i++) {
            if(fgets(line, 25, file) != NULL) {
                p.position[i] =  atoi(line);
            }
            else {
                p.position[i] = 0;
            }
        }
        for(i=0 ; i<INV_SIZE ; i++) {
            if(fgets(line, 25, file) != NULL) {
                char *array[2];
                int j=0;
                char *split = strtok(line, "-");
                while(split != NULL) {
                    array[j++] = split;
                    split = strtok(NULL, "-");
                }
                p.inventory[i].desc.type = getBlockType(array[0]);
                p.inventory[i].number = atoi(array[1]);
            }
            else {
                p.inventory[i].desc.type = NONE;
                p.inventory[i].number = 0;
            }
        }

        fclose(file);
    }

    return p;
}

bool savePlayer(player p) {

	FILE *file;
	char filePath[80];
	sprintf(filePath, "server/saves/%s.player", p.name);
	file = fopen(filePath, "w+");

	if(file != NULL) {
		fprintf(file, "%d\n%d\n", p.position[0], p.position[1]);
		int i=0;
		for(i=0 ; i<INV_SIZE ; i++) {
			fprintf(file, "%s-%d\n", getBlockName(p.inventory[i].desc.type), p.inventory[i].number);
		}

		fclose(file);
		return true;
	}

	return false;
}

bool savePlayers(player *p, int nbPlayers) {
    int i=0;
    int savesOk=0;
    for(i=0 ; i<nbPlayers ; i++) {
        if(savePlayer(p[i])) {
            savesOk ++;
        }
    }

    if(savesOk == nbPlayers) {
        return true;
    }
    return false;
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
