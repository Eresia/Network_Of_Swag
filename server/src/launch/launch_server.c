#include "launch_server.h"
#include "../../../data/src/map/Map.h"

/*Server arguments :
	1 : Port of communication
*/
int main(int argc, char** argv){

	Server server;
	Gameloop gl;
	ServerNetwork sn;
	pthread_t thread_gameloop, thread_network, thread_shell;
	pthread_mutex_t glStopMutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t glMapMutex = PTHREAD_MUTEX_INITIALIZER;
	int* descGl = malloc(2*sizeof(int));
	int* descSn = malloc(2*sizeof(int));

	if(argc != 2){
		printf("Not expected arguments\n");
		return BAD_NUMBER_OF_ARGUMENTS;
	}

	sn.port = atoi(argv[1]);

	if(sn.port == 0){
		printf("Number expected for port\n");
		return INCORRECT_ARGUMENT;
	}

	if((pipe(descGl) != 0) || (pipe(descSn) != 0)){
		printf("Error of pipe\n");
		return OTHER;
	}

	gl.isStopped = false;
	gl.thread = &thread_gameloop;
	gl.stopMutex = &glStopMutex;
	gl.mapMutex = &glMapMutex;
	gl.desc = descGl;
	gl.map = getMapFromFile("server/saves/static.map");
	gl.listPlayer = createListPlayer();

	sn.thread = &thread_network;
	sn.desc = descSn;

	//server = malloc(sizeof(Server));
	server.gl = gl;
	server.sn = sn;

	sem_unlink("/semGl");

	pthread_create(&thread_gameloop, NULL, launch_gameloop, &server);
	pthread_create(&thread_network, NULL, launch_network, &server);
	pthread_create(&thread_shell, NULL, launch_shell, &server);
	pthread_join(thread_gameloop, NULL);

	printf("Serveur stopped\n");


	return NO_ERROR;



}
