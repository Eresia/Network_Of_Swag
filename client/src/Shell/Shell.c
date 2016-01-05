#include "Shell.h"
#include "../network/Protocole.h"

void* launch_shell(void* client_void){

	Client* client = (Client*) client_void;
	char* cmd, *request;

	do{
		cmd = getString();
		#ifdef DEBUG
		printf("Enter message : %s\n", cmd);
		#endif
		request = Requete_Message(cmd);
		send_to_server(client->cn, request, strlen(request));

	}while(!client->isClosed);

	printf("CloseShell\n");
	pthread_exit(NULL);
}

char* getString(){
	char* string = calloc(100, sizeof(char));
	fgets(string, 100, stdin);
	clean(string, stdin);
	return string;
}
