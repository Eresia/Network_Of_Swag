#ifndef DEFINE_CLIENT
#define DEFINE_CLIENT

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

	//General includes
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>

	//Threads include
	#include <pthread.h>
	#include <semaphore.h>

	//Socket include
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>

	//For Windows or Linux :
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
		#define SIZE_MESSAGE_MAX 1500
		typedef struct sockaddr_in SOCKADDR_IN;
		typedef struct sockaddr SOCKADDR;
		typedef struct in_addr IN_ADDR;

	#endif

	//Boolean utilisation
	typedef enum bool bool;
	enum bool{
		false = 0, true = 1
	};


	typedef struct client_network_struct{
		pthread_t* client_beat_thread, recever_thread, writer_thread;
		sem_t*  write_buffer_full;
		sem_t* write_buffer_empty;
		sem_t* write_buffer_busy;
		sem_t* read_buffer_full;
		sem_t* read_buffer_empty;
		sem_t* read_buffer_busy;
		int  active_client;
		char serv_ip_addr[16];
		int	 serv_port;
		int  local_socket;
		char BUFF_IN[SIZE_MESSAGE_MAX];
		char BUFF_OUT[SIZE_MESSAGE_MAX];
		SOCKADDR_IN *serv_struct;
	} *client_network;


	client_network init_client_network(client_network c, char* serv_addr, int serv_port);
	int connect_to(char* serv_addr, int serv_port);
	void bufferize_from_server(void* client_struct);
	void bufferize_to_server(void* client_struct);
	int send_to_server(client_network client_struct, void* data , int size);
	int receive_from_server(client_network client_struct, void* dest);
	void launch_network(void*  client_struct);


#endif
