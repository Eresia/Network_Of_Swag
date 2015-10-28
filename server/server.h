#ifndef DEFINE_SERVER
#define DEFINE_SERVER

	//#define DEBUG
	#define NB_CLIENT_MAX 100

	/*Returns and errors :
		0 : No error
		1 : Bad number of arguments
		2 : Incorrect arguments
		3 : Other

	*/
	#define NO_ERROR 0
	#define BAD_NUMBER_OF_ARGUMENTS 1
	#define INCORRECT_ARGUMENT 2
	#define OTHER_ERROR

	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <pthread.h>

	typedef struct ClientVar* Client;
	struct ClientVar{
		int id;
		int source;
		char *ip;
	};

	typedef struct ServerVar* Server;
	struct ServerVar{
			int port;
			int connectedClient;
			Client* clients;
	};

	int launch_server(int , int );

	void* connectToClient(void* );






#endif
