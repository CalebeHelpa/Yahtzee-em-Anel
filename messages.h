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

#define MAXLINE 1024

// Estrutura para guardar todas as informações a respeito
// do DGRAM e sockets
typedef struct game_socket_t
{
    int baton;          // 1 se possui o bastao, 0 caso não

    int myPort;         // port em que o player está no anel
    int nxtPlayerPort;  // port do próximo player

    int sockfd;         // descritor do socket
    
    struct sockaddr_in myaddr;     // Endereço do player
    struct sockaddr_in nextaddr;   // Endereço do próximo player
    struct sockaddr_in prevaddr;   // Endereço do anterior (preenchido quando recebe)
} game_socket_t;

/***** Estrutura das mensagens *****/
// Marcado inicial: 8bits
#define START_MARKER 0xA

// Tipo: 1 byte
#define TYPE_CONF  'c'
#define TYPE_BATON 's'
#define TYPE_BET   'b'
#define TYPE_PLAY  'p'

typedef struct message_t
{
    int origin;
    char type;

    int bet_type;
    int bet;
    int result;
} message_t;

/***** Funções *****/
game_socket_t* create_game_socket (int myPort, int nxtPort);

message_t *receiveMessage(game_socket_t *g_socket);

int sendMessage(game_socket_t *g_socket, message_t *msg);

int getPort(int id);

message_t* create_message (int origin, char type);

void passBaton();

#endif