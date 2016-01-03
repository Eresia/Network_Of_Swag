#include "Process.h"

void move(Player* player, Move direction, block** map, int out){

	switch(direction){
		/*case BOT:
			if(canGoToBlock(player->position[0], player->position[1], 0, 1, map)){
				player->position[1]++;
			}
			else{
				#ifdef DEBUG
				printf("Bot direction not possible\n");
				#endif
			}
			break;*/
		case RIGHT:
			if(canGoToBlock(player->position[0], player->position[1], 1, 0, map)){
				player->position[0]++;
				char* request = Requete_Deplacement_Envoi(direction);
				write(out, request, strlen(request));
			}
			else{
				#ifdef DEBUG
				printf("Right direction not possible\n");
				#endif
			}
			break;
		case LEFT:
			if(canGoToBlock(player->position[0], player->position[1], -1, 0, map)){
				player->position[0]--;
				char* request = Requete_Deplacement_Envoi(direction);
				write(out, request, strlen(request));
			}
			else{
				#ifdef DEBUG
				printf("Left direction not possible\n");
				#endif
				char* request = Requete_Deplacement_Envoi(direction);
				write(out, request, strlen(request));
			}
			break;
		case TOP:
			if(!player->falling && (canGoToBlock(player->position[0], player->position[1], 0, -1, map))){
				player->position[1]--;
				char* request = Requete_Deplacement_Envoi(direction);
				write(out, request, strlen(request));
			}
			else{
				#ifdef DEBUG
				printf("Top direction not possible\n");
				#endif
			}
			break;
		default:
			#ifdef DEBUG
			printf("Direction not know\n");
			#endif
			break;
	}
}

void breakBlock(Player* player, int x, int y, block** map, int out){
	if(canAccesBlock(player->position[0], player->position[1], x, y, map, false)){
		block b = map[x][y];
		addBlockToInv(player, b);
		map[x][y].type = NONE;
	}
}
void putBlock(Player* player, int x, int y, int index, block** map, int out){
	if(canAccesBlock(player->position[0], player->position[1], x, y, map, true)){
		if((player->inventory[index].number != 0) && (player->inventory[index].desc.type != NONE)){
			map[x][y] = player->inventory[index].desc;
			removeBlockFromInv(player, index);
		}
	}
}
