#include "../network/server.h"

/*Server arguments :
	1 : Port of communication
	2 : Port of UE

*/
int main(int argc, char** argv){

	int nbArgc = 1, communicationPort, UE4Port;
	int resultServ;
	char* msg;

	if(argc != (nbArgc+1)){
		msg = "Bad number of arguments\n";
		write(1, msg, strlen(msg));
		#ifdef DEBUG
			printf("%s", msg);
		#endif
		return BAD_NUMBER_OF_ARGUMENTS;
	}

	communicationPort = atoi(argv[1]);
	UE4Port = 9999;

	if((communicationPort == 0) || (UE4Port == 0)){
		msg = "Incorrect arguments\n";
		write(1, msg, strlen(msg));
		#ifdef DEBUG
			printf("%s", msg);
		#endif
		return INCORRECT_ARGUMENT;
	}

	resultServ = launch_server(communicationPort, UE4Port);
	if(resultServ != NO_ERROR){
		msg = "Server stopped with errors\n";
		write(1, msg, strlen(msg));
		#ifdef DEBUG
			printf("%s", msg);
		#endif
		return resultServ;
	}

	msg = "Serveur stopped without error\n";
	printf("%s", msg);

	return NO_ERROR;



}
