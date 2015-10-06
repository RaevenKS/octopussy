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
  
  
  const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n Quae circumspecta exsertare crispare edita coeperint et edita observatum in terna crispare nec bina in est ob quod exsertare circumspecta ob rem ob in contingit milites nec nec nec quod pecudum adsidue observatum trucidantur nec trucidantur grassatores bina cedunt exsertare bina edita edita et milites coeperint cedunt cedunt observatum pecudum nec terna contingit et ob vel exsertare quae iniquitati deinceps ritu trucidantur vehunt trucidantur potuerint in cum iniquitati ubi loci ob iniquitati iniquitati observatum circumspecta nec crispare potuerint quod montium permissi quae tela pecudum edita crispare potuerint in vel trucidantur ritu montium bina crispare rem montium cedunt est cedunt exsertare.\n";
  int socket_serveur = creer_serveur(8081);
  int socket_client;
  char buffer[80];
  initialiser_signaux();
  while(1){
    socket_client = accept(socket_serveur, NULL, NULL);
    if (socket_client == -1){
	    perror("accept"); // Erreur sur l'acceptation
    }
	  
    int pid = fork();
    if(pid==0){
      sleep(1);
      write(socket_client, message_bienvenue, strlen(message_bienvenue));
      while(1){
	int i = read(socket_client, buffer, 80);
	if(i != -1)
	  write(socket_client, buffer, i);
	else
	  write(socket_client, buffer, 80);
      }
    }
    else{
      close(socket_client);
    }
  }
  return 0;
}
