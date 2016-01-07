#ifndef DEFINE_CLIENT
#define DEFINE_CLIENT

	/*General includes*/
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>
	#include <errno.h>
	/*Threads include*/
	#include <pthread.h>
	#include <semaphore.h>

	/*Socket include*/
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>

	/*For Windows or Linux :*/
	#ifdef WIN32

		#include <winsock2.h>

	#else

		#include <sys/types.h>
		#include <sys/socket.h>
		#include <netinet/in.h>
		#include <arpa/inet.h>
		#include <unistd.h> /* close */
		#include <netdb.h> /* gethostbyname */
		#define INVALID_SOCKET -1
		#define SOCKET_ERROR -1
		#define closesocket(s) close(s)
		typedef int SOCKET;
		typedef struct sockaddr_in SOCKADDR_IN;
		typedef struct sockaddr SOCKADDR;
		typedef struct in_addr IN_ADDR;

	#endif

	#include "../../../data/src/usefull/Usefull.h"

	typedef struct client_network_struct{
		bool*  isClosed;
		char serv_ip_addr[16];
		int	 serv_port;
		SOCKET  local_socket;
		char* BUFF_IN;
		char* BUFF_OUT;
		SOCKADDR_IN *serv_struct;
	} *client_network;


	client_network init_client_network(char* serv_addr, int serv_port);
	int connect_to(char* serv_addr, int serv_port);
	void* client_beat(void* client_struct);
	void* bufferize_from_server(void* client_struct);
	void* bufferize_to_server(void* client_struct);
	int send_to_server(client_network client_struct, void* data , int size);
	int receive_from_server(client_network client_struct, void* dest);
	void* launch_network(void* client_struct);


#endif
