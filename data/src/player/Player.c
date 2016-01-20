#include "Player.h"

Player* createPlayer(char *playerName) {
	Player* p = malloc(sizeof(Player));

	p->name = playerName;
	p->position = malloc(2*sizeof(int));
	p->position[0] = -1;
	p->position[1] = -1;
	p->falling = false;

	block b;
	b.type = NONE;
	invCase inv;
	inv.desc = b;
	inv.number = 0;

	int i=0;
	for(i=0 ; i<INV_SIZE ; i++) {
		p->inventory[i] = inv;
	}

	return p;
}

// Ajoute un bloc à l'inventaire et retourne si ça a marché ou pas (envoyer &player dans les paramètres)
bool addBlockToInv(Player *player, block block) {
	int index = caseNotFull(*player, block);

	if(index != -1) {
		player->inventory[index].number ++;
		return true;
	}
	else {
		int i=0;
		for(i=0 ; i<INV_SIZE ; i++) {
			if(player->inventory[i].number == 0) {
				player->inventory[i].desc = block;
				player->inventory[i].number ++;
				return true;
			}
		}
	}

	return false;
}

bool removeBlockFromInv(Player *player, int index) {
	if(player->inventory[index].number > 1) {
		player->inventory[index].number --;
		return true;
	}

	if(player->inventory[index].number > 0) {
		block noneBlock = {NONE, SKY};
		player->inventory[index].number = 0;
		player->inventory[index].desc = noneBlock;
		return true;
	}

	return false;
}

// Retourne l'index d'une case non pleine du bloc correspondant
int caseNotFull(Player player, block block) {
	int i=0;

	int *inventoryBlock = inInventory(player, block);

	for(i=0 ; i<INV_SIZE ; i++) {
		if((inventoryBlock[i] != -1) && (inventoryBlock[i] < MAX_PER_CASE)) {
			return i;
		}
	}

	return -1;
}

// Retourne un tableau du nombre de bloc (dont le type est celui passé en paramètre) par case de l'inventaire
int *inInventory(Player player, block block) {
	int i=0;
	int *indexTab = malloc(INV_SIZE * sizeof(int));

	for(i=0 ; i<INV_SIZE ; i++) {
		if(player.inventory[i].desc.type == block.type) {
			indexTab[i] = player.inventory[i].number;
		}
		else {
			indexTab[i] = -1;
		}
	}

	return indexTab;
}
