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
	int nbBlocX;
	int nbBlocY;

	nbBlocX = NB_LIGNE;
	nbBlocY = NB_COLONNE;

	blockVoid.type = VOID;
	blockVoid.back = SKY;

	for(i=0 ; i<nbBlocX ;i++){
		for(j = 0 ; j<nbBlocY ; j++){
			int mapX;
			int mapY;

			mapX = player->position[0] - (nbBlocX/2) +i;
			mapY = player->position[1] - (nbBlocY/2) + j;
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
}

void printBlock(SDL_Window* mw, block blocValue, int x, int y, bool absolute){
	SDL_Surface* surface;
	SDL_Rect block;

	surface = NULL;
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
	block.x = x;
	block.y = y;
	block.w = 24;
	block.h = 24;
	SDL_BlitSurface(surface,NULL,SDL_GetWindowSurface(mw), &block);
}

void printPlayer(SDL_Window* window, int x, int y){
	SDL_Rect playerRect;
	playerRect.x = x*24;
	playerRect.y = y*24;
	playerRect.w = 24;
	playerRect.h = 24;
	SDL_BlitSurface(playerBmp,NULL,SDL_GetWindowSurface(window), &playerRect);
}

void printInventory(SDL_Window* window, int selectedItem, Player* player){
	  int i;
	  int x;
	  int y;
	  SDL_Rect rect;

	  x = (WIDTH / 2) - 80;
	  y = HEIGHT - 40;
	  for(i=0 ; i<4; i++){
		rect.x = x + (i*40);
		rect.y = y;
		rect.w = 40;
		rect.h = 40;
	    SDL_BlitSurface(frameBmp,NULL,SDL_GetWindowSurface(window), &rect);
	  }
	  rect.x = x + (selectedItem*40);
	  rect.y = y;
	  rect.w = 40;
	  rect.h = 40;
	  SDL_BlitSurface(selectedBmp,NULL,SDL_GetWindowSurface(window), &rect);
	  for(i =0 ; i < 4 ; i++){
	    if(player->inventory[i].desc.type !=NONE){
	      int blocX;
	      int blocY;

		  blocX = x + (i * 40) + 8;
		  blocY = y + 8;
	      printBlock(window,player->inventory[i].desc, blocX, blocY, true);
	    }
  	}
}


bool waitEvent(SDL_Event event, SDL_Window* window, int* selectedItem, Player* player, block** map, client_network out){ /*Gére les différents évenement*/
	bool quit;
	int mouseX,mouseY;

	quit = false;
	if(SDL_WaitEventTimeout(&event, 50)){
		switch (event.type) {
			case SDL_WINDOWEVENT:
				if(event.window.event == SDL_WINDOWEVENT_CLOSE){
					quit = true;
					printf("Close window\n");
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT){
					SDL_GetMouseState(&mouseX,&mouseY);
					putBlock(player, mouseX/24 + player->position[0] - NB_LIGNE/2, mouseY/24 + player->position[1] - NB_COLONNE/2, *selectedItem, map, out);
					SDL_UpdateWindowSurface(window);
				}
				else if(event.button.button == SDL_BUTTON_RIGHT){
					SDL_GetMouseState(&mouseX,&mouseY);
					printf("pX : %d, pY : %d, mX : %d, mY : %d\n", player->position[0], player->position[1], mouseX, mouseY);
					breakBlock(player, mouseX/24 + player->position[0] - NB_LIGNE/2, mouseY/24 + player->position[1] - NB_COLONNE/2, map, out);
					SDL_UpdateWindowSurface(window);
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym){
					case SDLK_ESCAPE:
						quit = true;
						printf("Close window\n");
						break;
					case SDLK_LEFT:
					    move(player, LEFT, map, out);
					    break;
					case SDLK_RIGHT:
					    move(player, RIGHT, map, out);
					    break;
					case SDLK_UP:
					    move(player, TOP, map, out);
					    break;
					/*case SDLK_DOWN:
					    move(player, BOT, map, out);
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
	Uint32 colorkey;
	airBmp = SDL_LoadBMP("./client/src/graphic/image/air.bmp");
	voidBmp = SDL_LoadBMP("./client/src/graphic/image/void.bmp");
	dirtBmp = SDL_LoadBMP("./client/src/graphic/image/dirt.bmp");
	ironBmp = SDL_LoadBMP("./client/src/graphic/image/iron.bmp");
	stoneBmp = SDL_LoadBMP("./client/src/graphic/image/stone.bmp");
	woodBmp = SDL_LoadBMP("./client/src/graphic/image/wood.bmp");
	caveBmp = SDL_LoadBMP("./client/src/graphic/image/cave.bmp");
	playerBmp = SDL_LoadBMP("./client/src/graphic/image/player.bmp");
	colorkey = SDL_MapRGB(playerBmp->format, 0xFF, 0xFF, 0xFF);
	SDL_SetColorKey(playerBmp,SDL_TRUE,colorkey); /* Transparence du blanc*/
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
