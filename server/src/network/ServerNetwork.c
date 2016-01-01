#include "ServerNetwork.h"
#include "../game/Server.h"

void* launch_network(void* server_void){

	Server* server = (Server*) server_void;
	server->sn.clients = createList();
	bool stopServer;
	SOCKET socket_tcp, socket_udp, clientSocket;
	SOCKADDR_IN *clientInfo = malloc(sizeof(SOCKADDR_IN));
	int result;
	int lastId = -1;

	result = begin_listen(&socket_tcp, &socket_udp, server->sn.port);
	if(result != NO_ERROR){
		pthread_mutex_lock(server->gl.stopMutex);
		server->gl.isStopped = true;
		pthread_mutex_unlock(server->gl.stopMutex);
		pthread_exit(&result);
	}

	do{

		clientSocket = waitConnexion(socket_tcp, clientInfo, 1, 0);

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
			CheckClient cc;
			pthread_t thread_com, thread_check;
			pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

			cn.socket_tcp = clientSocket;
			cn.socket_udp = socket_udp;
			cn.id = lastId + 1;
			cn.isClosed = false;
			cn.info = clientInfo;
			cn.closeMutex = &mutex;
			cn.thread = &thread_com;

			cc.client = &cn;
			cc.list = server->sn.clients;

			if(pthread_create(&thread_com, NULL, begin_communication, &cc) != 0){
				#ifdef DEBUG
	              printf("Threads com not created\n");
	            #endif
			}
			else if(pthread_create(&thread_check, NULL, checkIfClientIsConnected, &cc) != 0){
				#ifdef DEBUG
	              printf("Threads check not created\n");
	            #endif
			}
			else{
				lastId++;
				addClient(server->sn.clients, &cn);
			}

		}


		pthread_mutex_lock(server->gl.stopMutex);
		stopServer = server->gl.isStopped;
		pthread_mutex_unlock(server->gl.stopMutex);
		usleep(50);
	}while(!stopServer);

	closeAll(server->sn.clients);
	closesocket(socket_tcp);
	closesocket(socket_udp);
	result = NO_ERROR;
	pthread_exit(&result);
}

//Fonction qui lance l'écoute du serveur
int begin_listen(SOCKET* server_tcp, SOCKET* server_udp, int port){

	SOCKADDR_IN info_tcp;
	SOCKADDR_IN info_udp;
	*server_tcp = socket(AF_INET, SOCK_STREAM, 0);
	*server_udp = socket(AF_INET, SOCK_DGRAM, 0);
	if((*server_udp == INVALID_SOCKET) || (*server_udp == INVALID_SOCKET))
	{
		#ifdef DEBUG
		printf("Error in socket creation\n");
		#endif
		return OTHER_ERROR;
	}

	#ifdef DEBUG
	printf("Socket creation successfull\n");
	#endif

	//On assigne les informations TCP de connexions au serveur
	info_tcp.sin_addr.s_addr = htonl(INADDR_ANY); /* Accept any adress */
	info_tcp.sin_family = AF_INET;
	info_tcp.sin_port = htons(port);

	//On assigne les informations UDP de connexions au serveur
	info_udp.sin_addr.s_addr = htonl(INADDR_ANY); /* Accept any adress */
	info_udp.sin_family = AF_INET;
	info_udp.sin_port = htons(port);

	//On bind le serveur en TCP sur le port s'il est disponible
	if(bind (*server_tcp, (SOCKADDR *) &info_tcp, sizeof(info_tcp)) == SOCKET_ERROR)
	{
		#ifdef DEBUG
		printf("Socket can't be bind\n");
		#endif
		return OTHER_ERROR;
	}

	//On bind le serveur en UDP sur le port s'il est disponible
	if(bind (*server_udp, (SOCKADDR *) &info_udp, sizeof(info_udp)) == SOCKET_ERROR)
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
	if(listen(*server_tcp, NB_CLIENT_MAX) == SOCKET_ERROR)
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
