#include "Gameloop.h"
#include "Server.h"
#include "Protocole.h"

void* launch_gameloop(void* server_void){

	Server* server = (Server*) server_void;
	Gameloop* gl = &server->gl;
	bool stopServer;
	pthread_t processingThread;

	gl->map = getMapFromFile("static.map");
	gl->listPlayer = createListPlayer();

	if(pthread_create(&processingThread, NULL, processing, gl) != 0){
		#ifdef DEBUG
		  printf("Threads processing not created\n");
		#endif
		pthread_mutex_lock(gl->stopMutex);
		gl->isStopped = true;
		pthread_mutex_unlock(gl->stopMutex);
		pthread_join(*server->sn.thread, NULL);
		pthread_exit(NULL);
	}

	do{
		pthread_mutex_lock(gl->stopMutex);
		stopServer = gl->isStopped;
		pthread_mutex_unlock(gl->stopMutex);
		usleep(50);
	}while(!stopServer);

	pthread_join(processingThread, NULL);
	pthread_join(*server->sn.thread, NULL);
	pthread_exit(NULL);

}
