
#define PORT 2222

#include "client.h"

int main(int argc, char** argv){
  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == INVALID_SOCKET)
  {
    printf("socket()");
    exit(0);
  }

  struct hostent *hostinfo = NULL;
  SOCKADDR_IN sin;
  const char *hostname = "localhost";

  hostinfo = gethostbyname(hostname); // on récupère les informations de l'hôte auquel on veut se connecter
  if (hostinfo == NULL) // l'hôte n'existe pas
  {
      printf( "Unknown host %s.\n", hostname);
      exit(EXIT_FAILURE);
  }

  sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr; // l'adresse se trouve dans le champ h_addr de la structure hostinfo
  sin.sin_port = htons(PORT); // on utilise htons pour le port
  sin.sin_family = AF_INET;

  if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
  {
      printf("connect()");
      exit(0);
  }

  int size;
  char* sizeChar = (char*) malloc(sizeof(char));
  char* buff;
  send(sock, "Bonjour", 7, 0);
  printf("Message envoyé\n");
  recv(sock, sizeChar, 1, 0);
  size = atoi(sizeChar);
  buff = (char*) malloc(size*sizeof(char));
  recv(sock, buff, size, 0);
  buff[size] = '\0';
  printf("Message reçu : %s\n", buff);

  closesocket(sock);

  return 0;
}
