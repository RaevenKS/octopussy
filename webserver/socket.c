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

	int socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_serveur == -1){
		perror("socket_serveur"); // Erreur
	}

	if (bind(socket_serveur, (struct sockaddr *)&sockaddr, sizeof (sockaddr)) == -1){
		perror("bind socker_serveur" ); // Erreur de lien
	}

	if(listen(socket_serveur, 10) == -1){
		perror("listen socket_serveur"); // Erreur d'écoute
	}


	int socket_client;
	socket_client = accept(socket_serveur, NULL, NULL);
	if (socket_client == -1){
		perror("accept"); // Erreur sur l'acceptation
	}

	const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n Quae circumspecta exsertare crispare edita coeperint et edita observatum in terna crispare nec bina in est ob quod exsertare circumspecta ob rem ob in contingit milites nec nec nec quod pecudum adsidue observatum trucidantur nec trucidantur grassatores bina cedunt exsertare bina edita edita et milites coeperint cedunt cedunt observatum pecudum nec terna contingit et ob vel exsertare quae iniquitati deinceps ritu trucidantur vehunt trucidantur potuerint in cum iniquitati ubi loci ob iniquitati iniquitati observatum circumspecta nec crispare potuerint quod montium permissi quae tela pecudum edita crispare potuerint in vel trucidantur ritu montium bina crispare rem montium cedunt est cedunt exsertare.\n";
	write(socket_client, message_bienvenue, strlen(message_bienvenue));

	return 0;
}
