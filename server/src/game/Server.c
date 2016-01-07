#include <stdio.h>
#include <stdlib.h>
#include "Server.h"
#include "../network/ServerNetwork.h"

#define MAX_LINE_SIZE 2000

Map* getMapFromFile(char *name) {

	char mapBlock;
	FILE *mapFile;
	block **map;
	Map* endMap;
	int x, y;
	int* spawn;
	char* filePath;
	block sky, cave, dirt, stone, wood, iron;

	endMap = malloc(sizeof(Map));
	endMap->name = name;
	spawn = malloc(2*sizeof(int));
	filePath = calloc(strlen("server/saves/.map") + strlen(name) + 1, sizeof(char));
	sprintf(filePath, "server/saves/%s.map", name);

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

	sky.type = NONE;
	sky.back = SKY;
	sky.type = NONE;
	sky.back = CAVE;
	sky.type = DIRT;
	sky.back = SKY;
	sky.type = STONE;
	sky.back = CAVE;
	sky.type = WOOD;
	sky.back = SKY;
	sky.type = IRON;
	sky.back = CAVE;

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
		return NULL;
	}

	endMap->map = map;
	endMap->spawn = spawn;
	return endMap;
}

void getFileFromMap(Map* map) {

	FILE *mapFile;
	char mapBlock;
	char* filePath;
	char* filePathTemp;
	int x, y;

	filePath = calloc(strlen("server/saves/.map") + strlen(map->name) + 1, sizeof(char));
	filePathTemp = calloc(strlen("server/saves/2.map") + strlen(map->name) + 1, sizeof(char));
	sprintf(filePath, "server/saves/%s.map", map->name);
	sprintf(filePathTemp, "server/saves/%s2.map", map->name);
	mapFile = fopen(filePathTemp, "w+");

	if(mapFile != NULL) {
		for(y=0 ; y<SIZE_MAX_Y ; y++) {
			for(x=0 ; x<SIZE_MAX_X ; x++) {
				/*mapBlock = fgetc(mapFile);*/
				switch(map->map[x][y].type) {
					case NONE:
						switch(map->map[x][y].back) {
							case SKY:
								if((x == map->spawn[0]) && (y == map->spawn[1])){
									mapBlock = '9';
								}
								else{
									mapBlock = '0';
								}
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
		rename(filePathTemp, filePath);
	}
	else{
		printf("Problem to loading the map\n");
	}
}

Player* loadPlayer(char *name) {
    Player* p;
    FILE *file;
    char filePath[80];

	p = createPlayer(name);
    sprintf(filePath, "server/saves/%s.player", name);
    file = fopen(filePath, "r");

    if(file != NULL) {
        char line[25];
        int i=0;

        for(i=0 ; i<2 ; i++) {
            if(fgets(line, 25, file) != NULL) {
                p->position[i] =  atoi(line);
            }
            else {
                p->position[i] = 0;
            }
        }
        for(i=0 ; i<INV_SIZE ; i++) {
            if(fgets(line, 25, file) != NULL) {
                char *array[2];
                int j;
                char *split;

				split = strtok(line, "-");
				j = 0;
                while(split != NULL) {
                    array[j++] = split;
                    split = strtok(NULL, "-");
                }
                p->inventory[i].desc.type = getBlockType(array[0]);
                p->inventory[i].number = atoi(array[1]);
            }
            else {
                p->inventory[i].desc.type = NONE;
                p->inventory[i].number = 0;
            }
        }

        fclose(file);
    }
	else{
		p->position = NULL;
	}

    return p;
}

bool savePlayer(Player p) {

	FILE *file;
	char filePath[80];
	sprintf(filePath, "server/saves/%s.player", p.name);
	file = fopen(filePath, "w+");

	if(file != NULL) {
		int i;
		fprintf(file, "%d\n%d\n", p.position[0], p.position[1]);
		for(i=0 ; i<INV_SIZE ; i++) {
			fprintf(file, "%s-%d\n", getBlockName(p.inventory[i].desc.type), p.inventory[i].number);
		}

		fclose(file);
		return true;
	}

	return false;
}

bool savePlayers(Player *p, int nbPlayers) {
    int i;
    int savesOk;

	savesOk=0;
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

void saveAllPlayer(ListPlayer* list){
	saveAllPlayer_Item(list->firstItem);
}

void saveAllPlayer_Item(ItemListPlayer item){
	if(item == NULL){
		#ifdef DEBUG
			printf("Player saved\n");
		#endif
	}
	else{
		savePlayer(*item->player);
		saveAllPlayer_Item(item->next);
	}
}

void displayMap(block **map) {
	int x, y;

	for(y=0 ; y<SIZE_MAX_Y ; y++) {
		for(x=0 ; x<SIZE_MAX_X ; x++) {
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
	int startX, startY, x, y;

	startX = 0;
	startY = 0;
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
