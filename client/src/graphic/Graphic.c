#include "Graphic.h"

SDL_Surface* airBmp;
SDL_Surface* dirtBmp;
SDL_Surface* ironBmp;
SDL_Surface* stoneBmp;
SDL_Surface* woodBmp;
SDL_Surface* caveBmp;
SDL_Surface* playerBmp;
SDL_Surface* frameBmp;
SDL_Surface* selectedBmp;
SDL_Surface* voidBmp;

void printMap(SDL_Window* window,block **map, int selectedItem, Player* player){
	int i,j;
	block currentBloc, blockVoid;
	int nbBlocX = NB_LIGNE;
	int nbBlocY = NB_COLONNE;

	blockVoid.type = VOID;
	blockVoid.back = SKY;

	for(i=0 ; i<nbBlocX ;i++){
		for(j = 0 ; j<nbBlocY ; j++){
			int mapX = player->position[0] - (nbBlocX/2) +i;
			int mapY = player->position[1] - (nbBlocY/2) + j;
			if((mapX >= 0) && (mapX < SIZE_MAX_X) && (mapY >= 0) && (mapY < SIZE_MAX_Y)){
				currentBloc = map[mapX][mapY];
			}
			else{
				currentBloc = blockVoid;
			}

			printBlock(window, currentBloc, i*24, j*24, false);
		}
	}
	printInventory(window,selectedItem,player);
	SDL_UpdateWindowSurface(window);
}

void printBlock(SDL_Window* mw, block blocValue, int x, int y, bool absolute){
	SDL_Surface* surface = NULL;
	switch (blocValue.type) {
		case NONE:
		switch (blocValue.back) {
			case SKY:
			surface = airBmp;
			break;
			case CAVE:
			surface = caveBmp;
			break;
		}
		break;
		case DIRT:
		surface = dirtBmp;
		break;
		case STONE:
		surface = stoneBmp;
		break;
		case WOOD:
		surface = woodBmp;
		break;
		case IRON:
		surface = ironBmp;
		break;
		case VOID:
		surface = voidBmp;
		break;
	}
	if(absolute == false){
		x = (x/24) * 24;
		y = (y/24) * 24;
	}
	SDL_Rect block = {x, y, 24, 24};
	SDL_BlitSurface(surface,NULL,SDL_GetWindowSurface(mw), &block);
}

void printPlayer(SDL_Window* window, int x, int y){
	SDL_Rect playerRect = {x, y, 24, 45};
	SDL_BlitSurface(playerBmp,NULL,SDL_GetWindowSurface(window), &playerRect);
}

void printInventory(SDL_Window* window, int selectedItem, Player* player){
	int i = 0;
	int x = (WIDTH / 2) - 80;
	int y = HEIGHT - 40;
	for(i=0 ; i<4; i++){
		SDL_Rect rect ={x +(i * 40), y, 40, 40};
		SDL_BlitSurface(frameBmp,NULL,SDL_GetWindowSurface(window), &rect);
	}
	SDL_Rect rect ={x +(40 * selectedItem), y, 40, 40};
	SDL_BlitSurface(selectedBmp,NULL,SDL_GetWindowSurface(window), &rect);
	int item = -1;
	for(i =0 ; i < INV_SIZE ; i++){
		switch (player->inventory[i].desc.type) {
			case DIRT:
			item = 0;
			break;
			case STONE:
			item = 1;
			break;
			case WOOD:
			item = 2;
			break;
			case IRON:
			item = 3;
			break;
			default :
			item = -1;
		}
		if(item != -1){
			int blocX = x + (item*40) + 8;
			int blocY = y + 8;
			printBlock(window,player->inventory[i].desc, blocX, blocY, true);
		}
	}
}

bool waitEvent(SDL_Event event, SDL_Window* window, int* selectedItem, Player* player){ //Gére les différents évenement
	bool quit = false;
	int mouseX,mouseY;

	//block air = {NONE, SKY};
	//block cave = {NONE, CAVE};
	//block dirt = {DIRT, SKY, true};
	//block stone = {STONE, CAVE, true};
	//block wood = {WOOD, CAVE};
	//block iron = {IRON, CAVE, true};
	if(SDL_WaitEvent(&event)){
		switch (event.type) {
			case SDL_WINDOWEVENT:
				if(event.window.event == SDL_WINDOWEVENT_CLOSE){
					quit = true;
					printf("Close window\n");
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT){ //Ajout d'un bloc sur la map pour tester
					SDL_GetMouseState(&mouseX,&mouseY);
					Requete_Pose_Bloc(mouseX/24, mouseY/24, *selectedItem);
					//printBlock(window,wood,mouseX,mouseY,false);
					SDL_UpdateWindowSurface(window);
				}
				else if(event.button.button == SDL_BUTTON_RIGHT){ //Supression d'un bloc pour tester
					SDL_GetMouseState(&mouseX,&mouseY);
					Requete_Casse_Bloc(mouseX/24, mouseY/24);
					//printBlock(window,air,mouseX,mouseY,false);
					SDL_UpdateWindowSurface(window);
				}
				break;
			case SDL_KEYDOWN:
				printf("%d\n", event.key.keysym.sym);
				switch (event.key.keysym.sym){
					case SDLK_ESCAPE:
						quit = true;
						printf("Close window\n");
						break;
					case SDLK_LEFT:
					    Requete_Deplacement_Envoi(LEFT);
					    break;
					case SDLK_RIGHT:
					    Requete_Deplacement_Envoi(RIGHT);
					    break;
					case SDLK_UP:
					    Requete_Deplacement_Envoi(TOP);
					    break;
					/*case SDLK_DOWN:
					    Requete_Deplacement_Envoi(BOT);
					    break;*/
					case '&':
						*selectedItem = 0;
						break;
					case 233:
						*selectedItem = 1;
						break;
					case '"':
						*selectedItem = 2;
						break;
					case '\'':
						*selectedItem = 3;
						break;
				}
				break;
			case SDL_MOUSEWHEEL :
				if(event.wheel.y == -1){
					*selectedItem = (*selectedItem + 1) % 4;
				}
				else if (event.wheel.y == 1){
					*selectedItem = (*selectedItem - 1) ;
					if(*selectedItem < 0){
						*selectedItem = 3;
					}
				}
				SDL_UpdateWindowSurface(window);
				break;
		}
	}

	return quit;
}


void loadBmp(){
	airBmp = SDL_LoadBMP("./client/src/graphic/image/air.bmp");
	voidBmp = SDL_LoadBMP("./client/src/graphic/image/void.bmp");
	dirtBmp = SDL_LoadBMP("./client/src/graphic/image/dirt.bmp");
	ironBmp = SDL_LoadBMP("./client/src/graphic/image/iron.bmp");
	stoneBmp = SDL_LoadBMP("./client/src/graphic/image/stone.bmp");
	woodBmp = SDL_LoadBMP("./client/src/graphic/image/wood.bmp");
	caveBmp = SDL_LoadBMP("./client/src/graphic/image/cave.bmp");
	playerBmp = SDL_LoadBMP("./client/src/graphic/image/player.bmp");
	Uint32 colorkey = SDL_MapRGB(playerBmp->format, 0xFF, 0xFF, 0xFF);
	SDL_SetColorKey(playerBmp,SDL_TRUE,colorkey); // Transparence du blanc
	frameBmp = SDL_LoadBMP("./client/src/graphic/image/frame.bmp");
	selectedBmp = SDL_LoadBMP("./client/src/graphic/image/selected.bmp");
}

void freeBmp(){
	SDL_FreeSurface(airBmp);
	SDL_FreeSurface(dirtBmp);
	SDL_FreeSurface(ironBmp);
	SDL_FreeSurface(stoneBmp);
	SDL_FreeSurface(woodBmp);
	SDL_FreeSurface(caveBmp);
	SDL_FreeSurface(playerBmp);
	SDL_FreeSurface(frameBmp);
	SDL_FreeSurface(selectedBmp);
}