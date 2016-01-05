#include "ServerNetwork.h"
#include "../game/Server.h"

void* launch_network(void* server_void){

	Server* server = (Server*) server_void;
	server->sn.clients = createList();
	bool stopServer;
	SOCKET socket;
	SOCKADDR_IN* clientInfo;
	int result;
	char* buff = calloc(SIZE_MESSAGE_MAX + 1, sizeof(char));
	pthread_t threadCheck;
	CheckClient cc;
	cc.clients = server->sn.clients;
	cc.map = server->gl.map;

	result = begin_listen(&socket, server->sn.port);
	if(result != NO_ERROR){
		pthread_mutex_lock(server->gl.stopMutex);
		server->gl.isStopped = true;
		pthread_mutex_unlock(server->gl.stopMutex);
		pthread_exit(&result);
	}

	if(pthread_create(&threadCheck, NULL, checkIfClientIsConnected, &cc)){
		#ifdef DEBUG
		  printf("Threads check not created\n");
		#endif
		result = OTHER;
		pthread_exit(&result);
	}

	do{

		clientInfo = waitConnexion(socket, buff, 1, 0);

		if(clientInfo != NULL){
			#ifdef DEBUG
			printf("Message received : %s\n", buff);
			#endif
			char* result = calloc(strlen(buff) + 1, sizeof(char));
			strcpy(result, buff);
			result = strtok(result, ",");
			if(strcmp(result, "new") == 0){
				#ifdef DEBUG
				printf("Nouveau client : %s\n", buff);
				#endif
				if(server->sn.clients->nb >= NB_CLIENT_MAX){
					#ifdef DEBUG
					printf("Server is full\n");
					#endif
				}
				else if(isInListByInfo(server->sn.clients, clientInfo)){
					#ifdef DEBUG
					printf("Client already connect\n");
					#endif
				}
				else{
					char* pseudo = malloc(11*sizeof(char));
					strncat(pseudo, strtok(NULL, ""), 11);
					pseudo = take_begin(pseudo, 11, FORBIDEN_CHAR, strlen(FORBIDEN_CHAR));
					if(strlen(pseudo) == 0){
						#ifdef DEBUG
						printf("Bad pseudo\n");
						#endif
					}
					else{
						#ifdef DEBUG
						printf("Pseudo : %s\n", pseudo);
						#endif
						if(isInListByName(server->sn.clients, pseudo)){
							#ifdef DEBUG
							printf("Pseudo already taken\n");
							#endif
						}
						else{
							ClientNetwork* cn = malloc(sizeof(ClientNetwork));
							pthread_t* thread_com;
							pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

							thread_com = malloc(sizeof(pthread_t));

							cn->socket = socket;
							cn->name = pseudo;
							cn->isClosed = false;
							cn->info = clientInfo;
							cn->closeMutex = &mutex;
							cn->thread = thread_com;
							cn->map = server->gl.map;
							cn->players = server->gl.listPlayer;
							cn->nbTry = 0;

							if(pthread_create(thread_com, NULL, begin_communication, cn) != 0){
								#ifdef DEBUG
					              printf("Threads com not created\n");
					            #endif
							}
							else{
								addClient(server->sn.clients, cn);
								char* string = calloc(4 + strlen(pseudo) + 1, sizeof(char));
								sprintf(string, "%s%s", pseudo, ",-1");
								write(server->gl.desc[1], string, strlen(string));
								Player** player = malloc(sizeof(Player*));
								read(server->sn.desc[0], player, sizeof(Player*));
								if(player == NULL){
									#ifdef DEBUG
						              printf("Network - Player not added in Gameloop\n");
						            #endif
									removeClient(server->sn.clients, cn);
								}
							}
						}
					}
				}
			}
			else{
				ClientNetwork* cn = getClientByInfo(server->sn.clients, clientInfo);
				char* cleanBuff = take_begin(buff, strlen(buff), FORBIDEN_CHAR, strlen(FORBIDEN_CHAR));
				if(cn == NULL){
					#ifdef DEBUG
					  printf("Unknow client\n");
					#endif
				}
				else{
					if(strcmp(cleanBuff, "hello") == 0){
						cn->nbTry = 0;
					}
					else{
						char* string = calloc(SIZE_MESSAGE_MAX + 11, sizeof(char));
						sprintf(string, "%s%c%s", cn->name, ',', cleanBuff);
						write(server->gl.desc[1], string, strlen(string));
					}
				}
			}

		}


		pthread_mutex_lock(server->gl.stopMutex);
		stopServer = server->gl.isStopped;
		pthread_mutex_unlock(server->gl.stopMutex);
		usleep(50);
	}while(!stopServer);

	//closeAll(server->sn.clients);
	closesocket(socket);
	result = NO_ERROR;
	pthread_exit(&result);
}

//Fonction qui lance l'écoute du serveur
int begin_listen(SOCKET* server_udp, int port){

	SOCKADDR_IN info_udp;
	*server_udp = socket(AF_INET, SOCK_DGRAM, 0);

	if(*server_udp == INVALID_SOCKET)
	{
		#ifdef DEBUG
		printf("Error in socket creation\n");
		#endif
		return OTHER_ERROR;
	}

	#ifdef DEBUG
	printf("Socket creation successfull\n");
	#endif

	//On assigne les informations UDP de connexions au serveur
	info_udp.sin_addr.s_addr = htonl(INADDR_ANY); /* Accept any adress */
	info_udp.sin_family = AF_INET;
	info_udp.sin_port = htons(port);

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

	return NO_ERROR;
}

SOCKADDR_IN* waitConnexion(SOCKET server, char* buff, int secTimeout, int uSecTimeout){
	int iResult;
	fd_set rfds;
	struct timeval tv;
	SOCKADDR_IN* info = malloc(sizeof(SOCKADDR_IN));
	int size;
	int n;

	FD_ZERO(&rfds);
	FD_SET(server, &rfds);

	tv.tv_sec = (long) secTimeout;
	tv.tv_usec = (long) uSecTimeout;

	size = sizeof(*info);

	iResult = select(server + 1, &rfds, (fd_set *) 0, (fd_set *) 0, &tv);
	if(iResult > 0)
	{
		if( (n = recvfrom(server, buff, SIZE_MESSAGE_MAX + 1, 0, (SOCKADDR *)info, (socklen_t*) &size)) < 0){
			#ifdef DEBUG
				printf("Reception failed\n");
			#endif
			return NULL;
		}
		buff[n] = '\0';
		return info;
	}
	else
	{
		return NULL;
	}

}
