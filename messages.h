#ifndef __MESSAGES__
#define __MESSAGES__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef struct game_socket_t
{
    int myPort;
    int nxtPlayerPort;

    int sockfd;
    
    struct sockaddr_in myaddr;
    struct sockaddr_in nextaddr;
    struct sockaddr_in prevaddr;
} game_socket_t;



#define MAXLINE 1024

game_socket_t* create_game_socket (int myPort, int nxtPort);

int receiveMessage(game_socket_t *g_socket, char *buffer);

int sendMessage(game_socket_t *g_socket, char *buffer);

int getPort(int id);

#endif