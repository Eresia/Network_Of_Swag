#include "game.h"

SDL_Surface* airBmp;
SDL_Surface* dirtBmp;
SDL_Surface* ironBmp;
SDL_Surface* stoneBmp;
SDL_Surface* woodBmp;
SDL_Surface* caveBmp;
SDL_Surface* playerBmp;

void printMap(SDL_Window* window,block **map, int x, int y){
  int i,j;
  block currentBloc;
  int nbBlocX = WIDTH/24 +1 ;
  int nbBlocY = HEIGHT/24 +1;
  for(i=0 ; i<nbBlocX ;i++){
    for(j = 0 ; j<nbBlocY ; j++){
      int mapX = x - (nbBlocX/2) +i;
      int mapY = y - (nbBlocY/2) + j;
      if((mapX >= 0) && (mapX < XMAXMAP) && (mapY >= 0) && (mapY < YMAXMAP)){
        currentBloc = map[mapX][mapY];
        printBlock(window, currentBloc, i*24, j*24);
      }
    }
  }
  SDL_UpdateWindowSurface(window);
}

void printBlock(SDL_Window* mw, block blocValue, int x, int y){
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
  }
  x = (x/24) * 24;
  y = (y/24) * 24;
  SDL_Rect block = {x, y, 24, 24};
  SDL_BlitSurface(surface,NULL,SDL_GetWindowSurface(mw), &block);
}

void printPlayer(SDL_Window* window, int x, int y){
  SDL_Rect playerRect = {x, y, 24, 45};
  SDL_BlitSurface(playerBmp,NULL,SDL_GetWindowSurface(window), &playerRect);
}

bool waitEvent(SDL_Event event, SDL_Window* window){ //Gére les différents évenement
  bool quit = false;
  int mouseX,mouseY;
  /*block air = {NONE, BLUE, false};
  block dirt = {DIRT, GREEN, true};
  block stone = {STONE, GRAY, true};
  block wood = {WOOD, BROWN, true};
  block iron = {IRON, ORANGE, true};*/

  block air = {NONE, SKY, false};
  block cave = {NONE, CAVE, false};
  //block dirt = {DIRT, SKY, true};
  //block stone = {STONE, CAVE, true};
  block wood = {WOOD, CAVE, true};
  //block iron = {IRON, CAVE, true};

  if(SDL_PollEvent(&event)){
    switch (event.type) {
      case SDL_WINDOWEVENT:
        if(event.window.event == SDL_WINDOWEVENT_CLOSE){
          quit = true;
        }
      case SDL_MOUSEBUTTONUP:
        if(event.button.button == SDL_BUTTON_LEFT){ //Ajout d'un bloc sur la map pour tester
          SDL_GetMouseState(&mouseX,&mouseY);
          printBlock(window,wood,mouseX,mouseY);
          SDL_UpdateWindowSurface(window);
        }
        else if(event.button.button == SDL_BUTTON_RIGHT){ //Supression d'un bloc pour tester
        SDL_GetMouseState(&mouseX,&mouseY);
        printBlock(window,air,mouseX,mouseY);
        SDL_UpdateWindowSurface(window);
        }
    }
  }

  return quit;
}


void loadBmp(){
  airBmp = SDL_LoadBMP("./client/src/graphic/image/air.bmp");
  dirtBmp = SDL_LoadBMP("./client/src/graphic/image/dirt.bmp");
  ironBmp = SDL_LoadBMP("./client/src/graphic/image/iron.bmp");
  stoneBmp = SDL_LoadBMP("./client/src/graphic/image/stone.bmp");
  woodBmp = SDL_LoadBMP("./client/src/graphic/image/wood.bmp");
  caveBmp = SDL_LoadBMP("./client/src/graphic/image/cave.bmp");
  playerBmp = SDL_LoadBMP("./client/src/graphic/image/player.bmp");
  Uint32 colorkey = SDL_MapRGB(playerBmp->format, 0xFF, 0xFF, 0xFF);
  SDL_SetColorKey(playerBmp,SDL_TRUE,colorkey);
}

void freeBmp(){
  SDL_FreeSurface(airBmp);
  SDL_FreeSurface(dirtBmp);
  SDL_FreeSurface(ironBmp);
  SDL_FreeSurface(stoneBmp);
  SDL_FreeSurface(woodBmp);
  SDL_FreeSurface(caveBmp);
  SDL_FreeSurface(playerBmp);
}
