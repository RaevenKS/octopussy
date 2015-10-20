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

enum http_method {
  HTTP_GET ,
  HTTP_UNSUPPORTED ,
};

typedef struct
{
  enum http_method method ;
  int major_version ;
  int minor_version ;
  char * url ;
} http_request ;

void traitement_signal(int sig)
{
  printf("Signal %d recu\n", sig);
  waitpid(-1, &sig, WNOHANG);
}


void initialiser_signaux()
{
  struct sigaction sa;
  sa.sa_handler = traitement_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if(sigaction(SIGCHLD, &sa, NULL) == -1)
    perror("sigaction(SIGCHLD)");
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    perror("signal");
}

char *fgets_or_exit(char *buffer, int size, FILE *stream)
{
  if(fgets(buffer, size, stream)==NULL)
    {
      fprintf(stdout, "Connexion interrompue\n");
      exit(1);
    }
  return buffer;
}

int parse_http_request ( const char *request_line , http_request * request )
{
  int tokcpt = 0;
  int ert = 1;
  char *token;
  char *str = strdup(request_line);
  token = strtok(str, " ");
  request->method = HTTP_UNSUPPORTED;
  while(token)
    {
      tokcpt++;
      if(tokcpt==1 && strcmp(token, "GET")==0)
	{
	  request->method = HTTP_GET;
	}
      if(tokcpt==2)
	{
	  request->url = token;
	}
      if(tokcpt==3)
	{
	  if(strcmp(token, "HTTP/1.0\r\n")==0)
	    {
	      request->major_version = 1;
	      request->minor_version = 0;
	    }
	  else if(strcmp(token, "HTTP/1.1\r\n")==0)
	    {
	      request->major_version = 1;
	      request->minor_version = 1;
	    }
	  else
	    ert = 0;
	}
      if(tokcpt >3)
	{
	  ert = 0;
	}
      token=strtok(NULL, " "); 
    }
  if(tokcpt < 3)
    {
      ert = 0;
    }
  return(ert);
}

  
void skip_header(FILE *client)
{
  int lignevide = 0;
  char buffer [80];
  char *str;
  while(lignevide==0)
    {
      str = fgets_or_exit(buffer, 80, client);
      if(str!=NULL && (strcmp(str, "\r\n")==0 || strcmp(str, "\n")==0))
	lignevide = 1;
    }
}

void send_status(FILE *client, int code, const char *reason_phrase)
{
  fprintf(client, "HTTP/1.1 %d %s\n", code, reason_phrase);
  fflush(client);
}

void send_response(FILE *client, int code, const char *reason_phrase,
		     const char *message_body)
{
  send_status(client, code, reason_phrase);
  fprintf(client, "Content-length: %d\r\n\r\n", (int)strlen(message_body));
  fflush(client);
  fprintf(client, message_body);
  fflush(client);
}
