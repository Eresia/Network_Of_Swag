#include "server.h"

int launch_server(int communicationPort, int UEPort){

  Server server = (Server) malloc(sizeof(struct ServerVar));
  bool StopServer = false;
  int result;
  int infoSize;
  SOCKET serverSocket, client;
  SOCKADDR_IN serverInformation, clientInformation;

  server->port = communicationPort;
  server->clients = (Client*) malloc(NB_CLIENT_MAX * sizeof(Client));

  result = begin_listen(&serverSocket, &serverInformation, communicationPort);
  if(result != NO_ERROR){
    return result;
  }

  infoSize = sizeof(clientInformation);

  #ifdef DEBUG
    printf("Waiting for clients\n");
  #endif
  client = accept(serverSocket, (SOCKADDR *)&clientInformation, &infoSize);

  if(client == INVALID_SOCKET){
    #ifdef DEBUG
      printf("No connected with the client\n");
    #endif
  }
  else{
    #ifdef DEBUG
      printf("Client connected\n");
    #endif

  }



  closesocket(serverSocket);
  return NO_ERROR;
}

void* connectToClient(void* client){

}

int begin_listen(SOCKET* server, SOCKADDR_IN* info, int port){

  //Socket creation
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

  info->sin_addr.s_addr = htonl(INADDR_ANY); /* Accept any adress */

  info->sin_family = AF_INET;

  info->sin_port = htons(port);

  if(bind (*server, (SOCKADDR *) info, sizeof(*info)) == SOCKET_ERROR)
  {
    #ifdef DEBUG
      printf("Socket can't be bind\n");
    #endif
    return OTHER_ERROR;
  }

  #ifdef DEBUG
    printf("Socket binded\n");
  #endif

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
