#include "Gameloop.h"

void* launch_gameloop(void* server_void){

	Server* server = (Server*) server_void;
	bool stopServer;

	do{
		pthread_mutex_lock(server->gl.stopMutex);
		stopServer = server->gl.isStopped;
		pthread_mutex_unlock(server->gl.stopMutex);
		usleep(50);
	}while(!stopServer);

	pthread_exit(NULL);
}

void makeCommand(char* command){

}
