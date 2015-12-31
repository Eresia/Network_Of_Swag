#include "game.h"

SDL_Surface* airBmp;
SDL_Surface* dirtBmp;
SDL_Surface* ironBmp;
SDL_Surface* stoneBmp;
SDL_Surface* woodBmp;

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
      surface = airBmp;
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

bool waitEvent(SDL_Event event, SDL_Window* window){ //Gére les différents évenement
  bool quit = false;
  int mouseX,mouseY;
  /*block air = {NONE, BLUE, false};
  block dirt = {DIRT, GREEN, true};
  block stone = {STONE, GRAY, true};
  block wood = {WOOD, BROWN, true};
  block iron = {IRON, ORANGE, true};*/

  block air = {NONE, SKY, false};
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
        if(event.button.button == SDL_BUTTON_LEFT){ //Ajout d'un bloc sur la map
          SDL_GetMouseState(&mouseX,&mouseY);
          printBlock(window,wood,mouseX,mouseY);
          SDL_UpdateWindowSurface(window);
        }
        else if(event.button.button == SDL_BUTTON_RIGHT){ //Supression du bloc qui est remplacé par un bloc d'air
        SDL_GetMouseState(&mouseX,&mouseY);
        printBlock(window,air,mouseX,mouseY);
        SDL_UpdateWindowSurface(window);
        }
    }
  }

  return quit;
}


void loadBmp(){
  airBmp = SDL_LoadBMP("./image/air.bmp");
  dirtBmp = SDL_LoadBMP("./image/dirt.bmp");
  ironBmp = SDL_LoadBMP("./image/iron.bmp");
  stoneBmp = SDL_LoadBMP("./image/stone.bmp");
  woodBmp = SDL_LoadBMP("./image/wood.bmp");
}

void freeBmp(){
  SDL_FreeSurface(airBmp);
  SDL_FreeSurface(dirtBmp);
  SDL_FreeSurface(ironBmp);
  SDL_FreeSurface(stoneBmp);
  SDL_FreeSurface(woodBmp);
}