// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "messages.h"

#define INITCHIPS 10

typedef struct player_t
{
  int playerID;
  int nxtPlayerID;

  int totalChips;
  int betValue;
  int betType;

} player_t ;


int get_player_id(int argc, char ** argv){
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

void initialize_player(player_t *player, int playerId){
    player->playerID = playerId;
    player->nxtPlayerID = playerId == 3 ? 0 : playerId + 1;
    player->totalChips = INITCHIPS;
    player->betValue = 0;
    player->betType = -1;
}


int main(int argc, char ** argv) { 
  // Configurações da máquina
  player_t player;
  int playerId = get_player_id(argc, argv);
  initialize_player(&player, playerId);

  
  game_socket_t *g_socket;
  g_socket = create_game_socket(getPort(player.playerID), getPort(player.nxtPlayerID));

  int cicle = 0;
  char buffer[MAXLINE];

  while(1){
    if(playerId > 0 || (playerId == 0 && cicle > 0)){
      // n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &prevaddr, &len);
      int n = receiveMessage(g_socket, buffer);
      buffer[n] = '\0';
      cicle = atoi(buffer);
      printf("Recebido: %d\n", cicle);
    }

    cicle++;
    char msg[MAXLINE];
    sprintf(msg, "%d", cicle);

    sendMessage(g_socket, msg); 
    printf("Enviado: %s\n", msg);
    printf("-----\n");
    sleep(2);
  }		
	return 0;
}