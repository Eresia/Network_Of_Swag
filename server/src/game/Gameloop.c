#include "Gameloop.h"
#include "Server.h"
#include "../../../data/src/map/Map.h"

void* launch_gameloop(void* server_void){

	Server* server = (Server*) server_void;
	bool stopServer;
	sem_t* closeGl;

	if((closeGl = sem_open("/semGl", O_CREAT, 0777)) == SEM_FAILED){
		printf("sem failed\n");
		perror(NULL);
		server->gl.isStopped = true;
		pthread_join(*server->sn.thread, NULL);
		pthread_exit(NULL);
	}

	block **map = getMapFromFile("static.map");

	do{
		sem_wait(closeGl);
		stopServer = server->gl.isStopped;
		sem_post(closeGl);
		usleep(50);
	}while(!stopServer);
	pthread_join(*server->sn.thread, NULL);
	pthread_exit(NULL);

}

void makeCommand(char* command){

}
