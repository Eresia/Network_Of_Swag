#include "Window.h"

void* launch_graphic(void * client_void){

	Client* client;
	Process* process;
	pthread_t threadDisplay;
	ScreenInfo* screenInfo;
	bool quit = false;

	client = (Client*) client_void;
	process = client->process;
	screenInfo = malloc(sizeof(ScreenInfo));
	if(SDL_Init(SDL_INIT_VIDEO) == -1){
		fprintf(stderr, "Erreur initialisation de la vidéo : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	else{
		SDL_Window* window;

		/*Création d'une fenêtre en pleine écran à la résolution de l'ordinateur*/
		window = SDL_CreateWindow("Network Of Swag", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);

		if(window){
			int selectedItem;
			SDL_Event event;
			Player* player;

			selectedItem = 0;
			loadBmp();

			if(client->logo){
				SDL_Surface* yogurt;
				SDL_Rect logo;

				yogurt = SDL_LoadBMP("./client/src/graphic/image/projet_reseau_logo.bmp");
				logo.x = 310;
				logo.y = 195;
				logo.w = 375;
				logo.h = 413;
				SDL_BlitSurface(yogurt,NULL,SDL_GetWindowSurface(window), &logo);
				SDL_UpdateWindowSurface(window);
				sleep(2);
			}

			player = process->player;
			screenInfo->win = window;
			screenInfo->process = process;
			screenInfo->selectedItem = &selectedItem;
			screenInfo->isClosed = &(client->isClosed);
			pthread_create(&threadDisplay, NULL, displayOnScreen, screenInfo);


			while(client->isClosed == false){
				/*Boucle principale*/
				quit = waitEvent(event,window,&selectedItem,player,process->map,client->cn); /* Gére les évenements*/
				if(quit){
					client->isClosed = true;
				}
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
	#ifdef DEBUG
	printf("Close Graphic\n");
	#endif
	pthread_exit(NULL);
}

void* displayOnScreen(void* screen_void){

	ScreenInfo* screen;
	Process* process;

	screen = (ScreenInfo*) screen_void;
	process = screen->process;

	while(*screen->isClosed == false){
		int i;
		printMap(screen->win, process->map, *screen->selectedItem, process->player);
		printInventory(screen->win, *screen->selectedItem, process->player);
		printPlayer(screen->win, (NB_LIGNE / 2), (NB_COLONNE / 2));
		for(i = 0; i < process->nbPlayers - 1; i++){
			printPlayer(screen->win, process->players[i].x - process->player->position[0] + (NB_LIGNE / 2), process->players[i].y - process->player->position[1] + (NB_COLONNE / 2));
		}
		SDL_UpdateWindowSurface(screen->win);
		usleep(1000000/30);
	}

	pthread_exit(NULL);
}
