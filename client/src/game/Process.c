#include "Process.h"
#include "../network/Protocole.h"

void move(Player* player, Move direction, block** map, client_network out){

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
				send_to_server(out, request, strlen(request));
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
				send_to_server(out, request, strlen(request));
			}
			else{
				#ifdef DEBUG
				printf("Left direction not possible\n");
				#endif
				char* request = Requete_Deplacement_Envoi(direction);
				send_to_server(out, request, strlen(request));
			}
			break;
		case TOP:
			if(!player->falling && (canGoToBlock(player->position[0], player->position[1], 0, -1, map))){
				player->position[1]--;
				char* request = Requete_Deplacement_Envoi(direction);
				send_to_server(out, request, strlen(request));
			}
			else{
				#ifdef DEBUG
				printf("falling : %d\n", player->falling);
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

void breakBlock(Player* player, int x, int y, block** map, client_network out){
	#ifdef DEBUG
	printf("Break block %d,%d", x, y);
	#endif
	if(canAccesBlock(player->position[0], player->position[1], x, y, map, false)){
		#ifdef DEBUG
		printf(" successfull\n");
		#endif
		block b = map[x][y];
		addBlockToInv(player, b);
		map[x][y].type = NONE;
		char* request = Requete_Casse_Bloc(x, y);
		send_to_server(out, request, strlen(request));
	}
	else{
		#ifdef DEBUG
		printf(" failed\n");
		#endif
	}
}
void putBlock(Player* player, int x, int y, int index, block** map, client_network out){
	#ifdef DEBUG
	printf("Put block %d,%d", x, y);
	#endif
	if(canAccesBlock(player->position[0], player->position[1], x, y, map, true)){
		if((player->inventory[index].number != 0) && (player->inventory[index].desc.type != NONE)){
			#ifdef DEBUG
			printf(" successfull\n");
			#endif
			map[x][y] = player->inventory[index].desc;
			removeBlockFromInv(player, index);
			char* request = Requete_Pose_Bloc(x, y, index);
			send_to_server(out, request, strlen(request));
		}
		else{
			#ifdef DEBUG
			printf(" failed\n");
			#endif
		}
	}
	else{
		#ifdef DEBUG
		printf(" failed\n");
		#endif
	}
}
