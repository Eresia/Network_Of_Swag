#include "server.h"

int launch_server(int communicationPort, int UEPort){

  Server server = (Server) malloc(sizeof(struct ServerVar));
  bool StopServer = false;
  int result;
  int infoSize;
  int clientPosition = 0, firstPosition, deltaPosition;
  bool serverFull;
  SOCKET serverSocket, clientSocket;
  SOCKADDR_IN serverInformation, clientInformation;
  pthread_t thread;
  char** delta = (char**) malloc(NB_DELTA_MAX * sizeof(char*));

  server->port = communicationPort;
  server->clients = (Client**) malloc(NB_CLIENT_MAX * sizeof(Client*));

  result = begin_listen(&serverSocket, &serverInformation, communicationPort);
  if(result != NO_ERROR){
    return result;
  }

  infoSize = sizeof(clientInformation);

  #ifdef DEBUG
    printf("Waiting for clients\n");
  #endif
  clientSocket = accept(serverSocket, (SOCKADDR *)&clientInformation, &infoSize);

  if(clientSocket == INVALID_SOCKET){
    #ifdef DEBUG
      printf("No connected with the client\n");
    #endif
  }
  else{
    #ifdef DEBUG
      printf("Client connected\n");
    #endif
    firstPosition = clientPosition;
    serverFull = false;
    while(server->clients[clientPosition] != NULL){
        clientPosition++;
        if(clientPosition == NB_CLIENT_MAX){
            clientPosition = 0;
        }
        if(clientPosition == firstPosition){
            printf("The server is full\n");
            serverFull = true;
        }
    }

    if(!serverFull){
        pthread_t speakThread;
        Client client;
        SpeakClient speak;
        client.id = clientSocket;
        client.thread = thread;
        client.isClosed = false;
        speak.client = client;
        speak.delta = delta;
        speak.startValue = &deltaPosition;
        server->clients[clientPosition] = &client;
        if((pthread_create(&thread, NULL, listenClient, &client) != 0) && (pthread_create(&speakThread, NULL, speakClient, &speak) != 0)){
            #ifdef DEBUG
              printf("Threads not created\n");
            #endif
            server->clients[clientPosition] = NULL;
        }
    }
    else{
        pthread_create(&thread, NULL, serverIsFull, &clientSocket);
    }


  }

  pthread_join(thread, NULL);

  closesocket(serverSocket);
  return NO_ERROR;
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



void* listenClient(void* clientVoid){
    Client* client = (Client*) clientVoid;
    char* buff = (char*) malloc(10*sizeof(char));
    recv(client->id, buff, 10, 0);
    printf("Message reçu : %s\n", buff);
    send(client->id, "Hello", 5, 0);
    printf("Message envoyé\n");

    closesocket(client->id);
    pthread_exit(NULL);
}

void* speakClient(void* clientVoid){
    SpeakClient* speakClient = (SpeakClient*) clientVoid;
    Client client = speakClient->client;
    int* value = speakClient->startValue;
    char** delta = speakClient->delta;

    while(true){

        char* result = ("\0");

        while(delta[*value] != NULL){
            realloc(result, strlen(result) + strlen(*delta[*value]));
            strcat(result, sprintf("%d%s", strlen(*delta[*value]), *delta[*value]));
            *value++;
        }

        if(!client.isClosed){
            #ifdef DEBUG
              printf("Sending : \"%s\" to the client\n", result);
            #endif
            send(client->id, result, strlen(result), 0);
            #ifdef DEBUG
              printf("Message sent", result);
            #endif

        }
        else{
            #ifdef DEBUG
              printf("Disconnected client\n");
            #endif
            break;
        }

    }
}

void* serverIsFull(void* clientVoid){
    #ifdef DEBUG
      printf("Server is full, client not accepted\n");
    #endif
    SOCKET* client = (SOCKET*) clientVoid;
    closesocket(*client);
    pthread_exit(NULL);
}
