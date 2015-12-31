#include "Shell.h"

void* launch_shell(void* server_void){

	Server* server = (Server*) server_void;
	bool stopServer;
	char* cmd;
	sem_t* closeGl;
	sem_t* closeAll;

	if((closeGl = sem_open("/semGl", O_CREAT, 0777)) == SEM_FAILED){
		printf("sem failed\n");
		perror(NULL);
		server->gl.isStopped = true;
		pthread_exit(NULL);
	}

	if((closeAll = sem_open("/semGl", O_CREAT, 0777)) == SEM_FAILED){
		printf("sem failed\n");
		perror(NULL);
		server->gl.isStopped = true;
		pthread_exit(NULL);
	}

	do{
		cmd = getString();
		#ifdef DEBUG
			printf("Enter command : %s\n", cmd);
		#endif
		if(strcmp(cmd, "stop") == 0){
			stopServer = true;
			sem_wait(closeAll);
			server->gl.isStopped = true;
			sem_post(closeAll);
	}
		else{
			sem_wait(closeGl);
			stopServer = server->gl.isStopped;
			sem_post(closeGl);
			if(!stopServer){
				usleep(50);
			}
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
