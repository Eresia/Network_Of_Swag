#include "Window.h"

void* launch_graphic(void * client_void){ //Main de test (Bruno)

	Client* client = (Client*) client_void;
	Process* process = client->process;
	pthread_t threadDisplay;
	ScreenInfo* screenInfo = malloc(sizeof(ScreenInfo));
	if(SDL_Init(SDL_INIT_VIDEO) == -1){
		fprintf(stderr, "Erreur initialisation de la vidéo : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);//
	}
	else{
		SDL_Window* window = NULL;

		//Création d'une fenêtre en pleine écran à la résolution de l'ordinateur
		window = SDL_CreateWindow("Network Of Swag", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);

		if(window){
			int selectedItem = 0;
			SDL_Event event;
			loadBmp();

			Player* player = process->player; //Player temporaire pour les test
			block stone, iron;
			stone.type = STONE;
			iron.type = IRON;
			/*addBlockToInv(player,iron); //Ajout de bloc pour tester la barre d'inventaire
			addBlockToInv(player,stone);

			printMap(window,map,27,37,selectedItem,player); //Print de test

			printPlayer(window, 550, 51);  //Only for test
			SDL_UpdateWindowSurface(window); //Only for test*/
			screenInfo->win = window;
			screenInfo->process = process;
			screenInfo->selectedItem = &selectedItem;
			screenInfo->isClosed = &(client->isClosed);
			pthread_create(&threadDisplay, NULL, displayOnScreen, screenInfo);


			while(client->isClosed == false){
				//Boucle principale

				client->isClosed = waitEvent(event,window,&selectedItem,player,process->map,client->cn); // Gére les évenements
			}

			pthread_join(threadDisplay, NULL);
			freeBmp();
			SDL_DestroyWindow(window);
		}
		else{
			fprintf(stderr, "Erreur création de la fenêtre : %s\n", SDL_GetError());
			exit(-1);
		}
	}

	SDL_Quit();
	pthread_exit(NULL);
}

void* displayOnScreen(void* screen_void){

	ScreenInfo* screen = (ScreenInfo*) screen_void;
	Process* process = screen->process;

	while(*screen->isClosed == false){
		int i;
		printMap(screen->win, process->map, *screen->selectedItem, process->player);
		printInventory(screen->win, *screen->selectedItem, process->player);
		printPlayer(screen->win, (NB_LIGNE / 2), (NB_COLONNE / 2)-1);
		for(i = 0; i < process->nbPlayers - 1; i++){
			printPlayer(screen->win, process->players[i].x - process->player->position[0] + (NB_LIGNE / 2), process->players[i].y - process->player->position[1] + (NB_COLONNE / 2));
		}
		SDL_UpdateWindowSurface(screen->win);
		usleep(1000000/30);
	}

	pthread_exit(NULL);
}
