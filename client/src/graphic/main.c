#include "game.h"

int main(){ //Main de test (Bruno)
  if(SDL_Init(SDL_INIT_VIDEO) == -1){
    fprintf(stderr, "Erreur initialisation de la vidéo : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);//
  }
  else{
    SDL_Window* window = NULL;

    //Création d'une fenêtre en pleine écran à la résolution de l'ordinateur
    window = SDL_CreateWindow("name", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,NULL);

    if(window){
      bool quit = false;
      SDL_Event event;
      loadBmp();

      block **map = getMapFromFile("./server/src/game/static.map"); //Map temporaire a partir des fonctions de vincent
      printMap(window,map,37,27);

      while(quit == false){
        //Boucle principale
        quit = waitEvent(event,window); // Gére les évenements

        //Actualisation de la map par le serveur
      }

      freeBmp();
      SDL_DestroyWindow(window);
    }
    else{
      fprintf(stderr, "Erreur création de la fenêtre : %s\n", SDL_GetError());
      exit(-1);
    }
  }

  SDL_Quit();
  return 0;
}
