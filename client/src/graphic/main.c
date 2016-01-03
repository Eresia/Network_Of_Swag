#include "game.h"

int main(){ //Main de test (Bruno)
  if(SDL_Init(SDL_INIT_VIDEO) == -1){
    fprintf(stderr, "Erreur initialisation de la vidéo : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);//
  }
  else{
    SDL_Window* window = NULL;

    //Création d'une fenêtre en pleine écran à la résolution de l'ordinateur
    window = SDL_CreateWindow("name", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);

    if(window){
      bool quit = false;
      int selectedItem = 0;
      SDL_Event event;
      loadBmp();

      Map* fullMap = getMapFromFile("./server/saves/static2.map"); //Map temporaire a partir des fonctions de vincent
      Player* player = createPlayer("TestBob"); //Player temporaire pour les test
      block stone, iron;
      stone.type = STONE;
      iron.type = IRON;
      addBlockToInv(player,iron); //Ajout de bloc pour tester la barre d'inventaire
      addBlockToInv(player,stone);

      block** map = fullMap->map;
      printMap(window,map,27,37,selectedItem,player); //Print de test

      printPlayer(window, 550, 51);  //Only for test
      SDL_UpdateWindowSurface(window); //Only for test

      while(quit == false){
        //Boucle principale
        quit = waitEvent(event,window,&selectedItem,player); // Gére les évenements

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
