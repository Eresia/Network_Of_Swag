#include "Gameloop.h"
#include "Server.h"
#include "../../../data/src/map/Map.h"

void* launch_gameloop(void* server_void){

	Server* server = (Server*) server_void;
	bool stopServer;

	block **map = getMapFromFile("static.map");

	do{
		pthread_mutex_lock(server->gl.stopMutex);
		stopServer = server->gl.isStopped;
		pthread_mutex_unlock(server->gl.stopMutex);
		usleep(50);
	}while(!stopServer);

	pthread_join(*server->sn.thread, NULL);
	pthread_exit(NULL);

}

void makeCommand(char* command){

}
