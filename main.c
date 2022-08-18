// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "messages.h"
#include "game.h"

void printMessage(message_t *msg){
    printf("Origin: %d", msg->origin);
    printf("\nType: %c", msg->type);
    printf("\nBet type: %d", msg->bet_type);
    printf("\nBet : %d", msg->bet);
    printf("\nResult : %d\n", msg->result);
}


int read_options(int argc, char ** argv);

int main(int argc, char ** argv) { 
  // Configurações da máquina
  player_t player;
  int playerId = read_options(argc, argv);
  initialize_player(&player, playerId);

  
  game_socket_t *g_socket;
  g_socket = create_game_socket(getPort(player.playerID), getPort(player.nxtPlayerID));

  int cicle = 0;
  char buffer[MAXLINE];

  while(1){
    if(playerId > 0 || (playerId == 0 && cicle > 0)){
      message_t *msg = receiveMessage(g_socket);
      if(msg->type == TYPE_BATON){
        receiveBaton(g_socket, msg);
        if(g_socket->baton == 1)
          printf("Bastão recebido !\n");
      } else {
        // printMessage(msg);
        printf("Recebido: %d\n", msg->bet);
        cicle = msg->bet;
        if(g_socket->baton == 1){
          printf("Passando bastao \n");
          passBaton(g_socket);
        }
      }
    }

    cicle++;
    message_t *msg_send = create_message(player.playerID, TYPE_BET);
    msg_send->bet = cicle;

    sendMessage(g_socket, msg_send); 
    printf("Enviado: %d\n", cicle);
    printf("-----\n");
    sleep(2);
  }		
	return 0;
}

int read_options(int argc, char ** argv){
  int option;
  opterr = 0;
  optind = 1;

  int id;
  while ((option = getopt (argc, argv, "i:")) != -1){
		switch (option)
		{
			case 'i':
        id = atoi(optarg);
        break;
			default:
        fprintf(stderr, "Usage: -i [player id]\n");
				exit(-1);
		}
	}

  return id;
}