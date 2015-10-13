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



void traitement_signal(int sig){
  printf("Signal %d recu\n", sig);
  waitpid(-1, &sig, WNOHANG);
}


void initialiser_signaux(){
  struct sigaction sa;
  sa.sa_handler = traitement_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if(sigaction(SIGCHLD, &sa, NULL) == -1)
    perror("sigaction(SIGCHLD)");
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    perror("signal");
}


int main(){
  
  
  const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";
  int socket_serveur = creer_serveur(8080);
  int socket_client;
  char buffer[80];
  FILE * file;
  char *str = "poule";
  char *token;
  int ok = 1;
  int tokcpt = 0;
  int lignevide = 0;
  
  initialiser_signaux();
  while(1){
    socket_client = accept(socket_serveur, NULL, NULL);
    if (socket_client == -1){
	    perror("accept"); // Erreur sur l'acceptation
    }

    
    int pid = fork();
    if(pid==0){
      file = fdopen(socket_client, "w+");
      if(file == NULL)
	perror("fdopen");
      str = fgets(buffer, 80, file);
      token = strtok(str, " ");
      while(token)
	{
	  tokcpt++;
	  if(tokcpt==1 && strcmp(token, "GET")!=0){
	      ok = 0;
	    }
	  if(tokcpt >3){
	      ok = 0;
	    }
	  if(tokcpt==3 && strcmp(token, "HTTP/1.0\r\n")!=0 && (strcmp(token, "HTTP/1.1\r\n")!=0)){
	    ok = 0;
	  }
	  token=strtok(NULL, " "); 
	}
      if(tokcpt < 3){
	  ok = 0;
	}
      while(lignevide==0)
	{
	  str = fgets(buffer, 80, file);
	  if(str!=NULL && (strcmp(str, "\r\n")==0 || strcmp(str, "\n")==0))
	    lignevide = 1;
	}
      if(ok==1){
	fprintf(file, "<Octopussy> %s\n HTTP/1.1 200 OK\r\nConnection: open\r\nContent-length:%d\r\n\n200 OK\n", message_bienvenue, (int)strlen(message_bienvenue));
	fflush(file);
      }
      else{
	fprintf(file, "<Octopussy>HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-length:17\r\n\n400 Bad Request\n ");
	fflush(file);
      }
      exit(0);
    }
    else{
      close(socket_client);
    }
  }
  return 0;
}
