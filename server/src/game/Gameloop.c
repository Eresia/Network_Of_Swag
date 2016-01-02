#include "Gameloop.h"
#include "Server.h"
#include "Protocole.h"

void* launch_gameloop(void* server_void){

	Server* server = (Server*) server_void;
	Gameloop* gl = &server->gl;
	bool stopServer;
	char* buff;

	gl->map = getMapFromFile("server/saves/static.map");
	gl->listPlayer = createListPlayer();

	do{
		buff = waitMessage(gl->desc[0], 1, 0);

		if(buff != NULL){
			char* pseudo = calloc(11, sizeof(char));
			char* cmd = calloc(SIZE_MESSAGE_MAX + 1, sizeof(char));
			pseudo = strtok(buff, ",");
			if(pseudo == NULL){
				#ifdef DEBUG
				printf("Bad string - pseudo (Gameloop)\n") ;
				#endif
			}
			else{
				cmd = strtok(NULL, "");
				if(cmd == NULL){
					#ifdef DEBUG
					printf("Bad string - cmd (Gameloop)\n") ;
					#endif
				}
				else{
					parse_Protocole(pseudo, cmd, gl, server->sn.desc[1]);
				}
			}
		}

		pthread_mutex_lock(gl->stopMutex);
		stopServer = gl->isStopped;
		pthread_mutex_unlock(gl->stopMutex);
		usleep(50);
	}while(!stopServer);

	pthread_join(*server->sn.thread, NULL);
	pthread_exit(NULL);

}

char* waitMessage(int out, int secTimeout, int uSecTimeout){
	int iResult;
	fd_set rfds;
	struct timeval tv;
	int n;
	char* buff = calloc(SIZE_MESSAGE_MAX + 11, sizeof(char));

	FD_ZERO(&rfds);
	FD_SET(out, &rfds);

	tv.tv_sec = (long) secTimeout;
	tv.tv_usec = (long) uSecTimeout;

	iResult = select(out + 1, &rfds, (fd_set *) 0, (fd_set *) 0, &tv);
	if(iResult > 0)
	{
		if((n = read(out, buff, SIZE_MESSAGE_MAX + 11)) < 0){
			#ifdef DEBUG
			printf("Gameloop : Message not received\n");
			#endif
			return NULL;
		}
		else{
			buff[n] = '\0';
			#ifdef DEBUG
			printf("Gameloop : Messages : %s\n", buff);
			#endif
			return buff;
		}
	}
	else
	{
		return NULL;
	}
}
