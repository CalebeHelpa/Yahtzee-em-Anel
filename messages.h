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
#define TYPE_CONF   'c'
#define TYPE_BATON  's'
#define TYPE_BET    'b'
#define TYPE_PLAY   'p'
#define TYPE_CHIPS  'r'

typedef struct message_t
{
    int origin;
    int player;
    char type;

    int bet_type;
    int bet;
    int result;
} message_t;

/***** Funções *****/

/*Função cria um objeto com as configurações necessário para o player acessar a rede*/
game_socket_t* create_game_socket (int myPort, int nxtPort);

int getPort(int id);

/***** MENSAGENS ******/
/*Função para cria uma nova mensagem com origem e tipo definidos
   demais campos são iniciados com o valor default 0*/
message_t* create_message (int origin, char type);

/*Espera receber uma mensagem pelo socket, enquanto o programa não
   receber nada ficará travado nessa função.
   Retorna a mensagem corretamente formatada*/
message_t *receiveMessage(game_socket_t *g_socket);

/*Envia uma mensagem para o próximo player na rede*/
int sendMessage(game_socket_t *g_socket, message_t *msg);

/*Combinação da função sendMessage e receiveMessage
  Envia mensagem e só saí quando obter a resposta */
message_t *sendAndWait(game_socket_t *g_socket, message_t *msg);

/***** BASTÃO ******/
/*Passa o bastão para o próximo jogador*/
void passBaton(game_socket_t *g_socket, int playerId);

/*Recebe o bastão ou simplesmente repassa a mensagem para a rede*/
void receiveBaton(game_socket_t *g_socket, message_t *msg);

/*Retorna 1 se tiver o bastão 0 caso contrário*/
int myMove (game_socket_t *g_socket);

#endif