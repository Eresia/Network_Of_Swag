#include "Shell.h"

void* launch_shell(void* server_void){

	Server* server = (Server*) server_void;
	bool stopServer;
	char* cmd;

	do{
		cmd = getString();
		#ifdef DEBUG
		printf("Enter command : %s\n", cmd);
		#endif
		if(strcmp(cmd, "stop") == 0){
			stopServer = true;
			pthread_mutex_lock(server->gl.stopMutex);
			server->gl.isStopped = true;
			pthread_mutex_unlock(server->gl.stopMutex);
		}
		else{
			if(strcmp(cmd, "nb") == 0){
				printf("Nombre de joueur : %d\n", server->sn.clients->nb);
			}
			usleep(50);
		}
	}while(!stopServer);
	pthread_exit(NULL);
}

char* getString(){
	char* string = calloc(100, sizeof(char));
	fgets(string, 100, stdin);
	clean(string, stdin);
	return string;
}
