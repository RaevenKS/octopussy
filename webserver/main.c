#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

int main(){

	creer_serveur(8080);
	return 0;
}
