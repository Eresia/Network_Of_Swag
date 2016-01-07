#include "CommonProtocol.h"

bool canGoToBlock(int pX, int pY, int dX, int dY, block** map){
	if(((pX + dX) >= 0) && ((pX + dX) < SIZE_MAX_X) && ((pY + dY) >= 0) && ((pY + dY) < SIZE_MAX_Y)){
		return (map[pX + dX][pY + dY].type == NONE);
	}
	else{
		return false;
	}
}

bool canAccesBlock(int pX, int pY, int bX, int bY, block** map, bool air){
	int absX = abs(pX - bX);
	 int absY = abs(pY - bY);
	if(((absX <= 1) && (absY <= 1)) && (absX != absY)){
		if((bX >= 0) && (bX < SIZE_MAX_X) && (bY >= 0) && (bY < SIZE_MAX_Y)){
			return (((map[bX][bY].type == NONE) && air) || ((map[bX][bY].type != NONE) && !air));
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}

void* fall(void* player_void){
	FallData* data = (FallData*) player_void;
	Player* player = data->player;
	block** map = data->map;
	do{
		usleep(500000);
		if(map[player->position[0]][player->position[1]+1].type == NONE){
			/*#ifdef DEBUG
			printf("%s is falling\n", player->name);
			#endif*/
			player->position[1]++;
			if(map[player->position[0]][player->position[1]+1].type != NONE){
				player->falling = false;
			}
		}
		else{
			player->falling = false;
		}
	}while(player->falling);

	pthread_exit(NULL);
}
