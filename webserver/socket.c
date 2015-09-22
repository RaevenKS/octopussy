#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>


int creer_serveur(int port){

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET; // Socket IPV4
	sockaddr.sin_port = htons(port); //Port d'écoute
	sockaddr.sin_addr.s_addr = INADDR_ANY; // Ecoute sur toutes les interfaces
	int optval = 1;
	int socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
	
	if(socket_serveur == -1){
	  perror("socket_serveur"); // Erreur
	}

	if (setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int))== -1){
	  perror("Can not set SO_REUSEADDR option");
	}
	
	if (bind(socket_serveur, (struct sockaddr *)&sockaddr, sizeof (sockaddr)) == -1){
	  perror("bind socket_serveur" ); // Erreur de lien
	}

	if(listen(socket_serveur, 10) == -1){
	  perror("listen socket_serveur"); // Erreur d'écoute
	}
	return socket_serveur;
}
