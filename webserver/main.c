#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <sys/wait.h>
#include "serveur.h"

int main(){
  const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";
  int socket_serveur = creer_serveur(8080);
  int socket_client;
  char buffer[80];
  FILE * client;
  char *str = "";
  int bad_request;
  http_request request;
  
  initialiser_signaux();
  while(1)
    {
    socket_client = accept(socket_serveur, NULL, NULL);
    if (socket_client == -1){
      perror("accept"); // Erreur sur l'acceptation
    }
    int pid = fork();
    if(pid==0)
      {
	client = fdopen(socket_client, "w+");
	if(client == NULL)
	  perror("fdopen");
	str = fgets_or_exit(buffer, 80, client);
	bad_request = parse_http_request(str, &request);
	skip_header(client);
	if (bad_request==0)
	  send_response(client, 400, "Bad Request", "Bad request\r\n");
	else if (request.method == HTTP_UNSUPPORTED)
	  send_response(client, 405, "Method Not Allowed", "Method Not Allowed\r\n");
	else if (strcmp(request.url, "/") == 0)
	  send_response(client, 200, "OK", message_bienvenue);
	else
	  send_response(client, 404, "Not Found", "Not Found\r\n");
	fprintf(client, "Poule");
	fflush(client);
	exit(0);
      }
    else
      {
	close(socket_client);
      }
    }
  return 0;
}
