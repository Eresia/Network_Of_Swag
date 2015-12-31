#include "ServerNetwork.h"

void* launch_network(void* server_void){

	Server* server = (Server*) server_void;
	server->sn.clients = createList();
	bool stopServer;
	SOCKET socket, clientSocket;
	SOCKADDR_IN *clientInfo = malloc(sizeof(SOCKADDR_IN));
	int result;
	int lastId = -1;

	result = begin_listen(&socket, server->sn.port);
	if(result != NO_ERROR){
		pthread_exit(&result);
	}

	do{

		clientSocket = waitConnexion(socket, clientInfo, 1, 0);

		if(clientSocket == INVALID_SOCKET){
			#ifdef DEBUG
			printf("No connected with the client\n");
			#endif
		}
		else if(clientSocket == TIMEOUT){
			#ifdef DEBUG
			//printf("Timeout\n");
			#endif
		}
		else{
			#ifdef DEBUG
			printf("Client connected\n");
			#endif
			ClientNetwork cn;
			pthread_t thread;
			pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

			lastId++;

			cn.socket = clientSocket;
			cn.id = lastId;
			cn.isClosed = false;
			cn.info = clientInfo;
			cn.closeMutex = &mutex;
			cn.thread = &thread;
		}


		pthread_mutex_lock(server->gl.stopMutex);
		stopServer = server->gl.isStopped;
		pthread_mutex_unlock(server->gl.stopMutex);
		usleep(50);
	}while(!stopServer);

	closesocket(socket);
	result = NO_ERROR;
	pthread_exit(&result);
}

//Fonction qui lance l'écoute du serveur
int begin_listen(SOCKET* server, int port){

	SOCKADDR_IN info;
	*server = socket(AF_INET, SOCK_STREAM, 0);
	if(*server == INVALID_SOCKET)
	{
		#ifdef DEBUG
		printf("Error in socket creation\n");
		#endif
		return OTHER_ERROR;
	}

	#ifdef DEBUG
	printf("Socket creation successfull\n");
	#endif

	//On assigne les informations de connexions au serveur
	info.sin_addr.s_addr = htonl(INADDR_ANY); /* Accept any adress */
	info.sin_family = AF_INET;
	info.sin_port = htons(port);

	//On bind le serveur sur le port s'il est disponible
	if(bind (*server, (SOCKADDR *) &info, sizeof(info)) == SOCKET_ERROR)
	{
		#ifdef DEBUG
		printf("Socket can't be bind\n");
		#endif
		return OTHER_ERROR;
	}

	#ifdef DEBUG
	printf("Socket binded\n");
	#endif

	//On commence à écouter
	if(listen(*server, NB_CLIENT_MAX) == SOCKET_ERROR)
	{
		#ifdef DEBUG
		printf("Error while listen\n");
		#endif
		return OTHER_ERROR;
	}

	#ifdef DEBUG
	printf("Server is listening\n");
	#endif

	return NO_ERROR;
}

SOCKET waitConnexion(SOCKET server, SOCKADDR_IN *info, int secTimeout, int uSecTimeout){
	int infoSize, iResult;
	fd_set rfds;
	struct timeval tv;

	FD_ZERO(&rfds);
	FD_SET(server, &rfds);

	tv.tv_sec = (long) secTimeout;
	tv.tv_usec = (long) uSecTimeout;

	infoSize = sizeof(*info);

	iResult = select(server + 1, &rfds, (fd_set *) 0, (fd_set *) 0, &tv);
	if(iResult > 0)
	{
		return accept(server, (SOCKADDR *) info, (socklen_t*) &infoSize);
	}
	else
	{
		return TIMEOUT;
	}

}
