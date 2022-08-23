// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "messages.h"
#include "game.h"

// Remover
void printMessage(message_t *msg){
    printf("Origin: %d", msg->origin);
    printf("\nPlayer: %d", msg->player);
    printf("\nType: %c", msg->type);
    printf("\nBet type: %d", msg->bet_type);
    printf("\nBet : %d", msg->bet);
    printf("\nResult : %d\n", msg->result);
}

/******** FUNÇÕES CONTROLE ********/
/* Le as opções passadas para o programa */
int read_options(int argc, char ** argv);

/* Função para dar tempo de iniciar todos os jogadores antes de iniciar*/
void pressToStart();

/* Envia mensagem de configuração:
   1: Inicio nova rodada
  -1: Fim de jogo*/
void sendConf(player_t *player, game_socket_t *g_socket, int type);

/* Recebe a configuração do jogo*/
int receiveConf(player_t *player, game_socket_t *g_socket);

/******** FUNÇÕES PLAYER COM BASTÃO ********/
/* Função para determinar qual a configuração da rodada
   Retorna o tipo da aposta, a posta e o jogador que irá realizar a jogada */
void defineBet(player_t *player, game_socket_t *g_socket, int *betType, int *betValue, int *playerBet);

/* Envia mensagem para quem irá jogar ou realiza a jogada */
void sendPlay(player_t *player, game_socket_t *g_socket, int betType, int betValue, int playerBet, int *result);

/* Envia mensagem para atualizar o saldo */
void sendNewChips(player_t *player, game_socket_t *g_socket, int playerBet);


/******** FUNÇÕES PLAYER SEM BASTÃO ********/
/* Recebe a aposta atual e determina se irá ou não apostar */
void receiveBet(player_t *player, game_socket_t *g_socket);

/* Recebe as instruçõs para jogar
  se deve jogar, faz a aposta e altera o campo de resultado da mensagem
  caso contrário apenas repassa a menssagem */
void receivePlay(player_t *player, game_socket_t *g_socket);

/* Recebe o novo saldo, atualiza e repassa a mensagem */
void receiveNewChips(player_t *player, game_socket_t *g_socket);

int main(int argc, char ** argv) { 
  // Configurações da máquina
  player_t player;
  int playerId = read_options(argc, argv);

  // Inicia as estruturas do jogador
  initialize_player(&player, playerId);

  // Inicia estrutura da rede em anel
  game_socket_t *g_socket;
  g_socket = create_game_socket(getPort(player.playerID), getPort(player.nxtPlayerID));

  // TODO: Mensagem para sincronizar enquanto abre (Pressiona s para iniciar o jogo)

  // Inicia jogo 
  while(1){
      int isMyMove = myMove(g_socket);

      /* TODO: Adicionar endGame
      if(isMyMove){
        // Verificar se o jogo acabou
        if(endGame()) {
          sendConf(&player, g_socket, -1);
          printf("\nFim de jogo\n");
          break;
        } else {
          sendConf(&player, g_socket, 1);
        }
      } else {
        int conf = receiveConf(&player, g_socket);
        if(conf == -1){
          printf("\nFim de jogo\n");
          break;
        } 
      }
      */

      // Game
      int betType, bet, player_biggest_bet, result;
      if(isMyMove) {
          // Define qual a jogada e quem vai jogar
          defineBet(&player, g_socket, &betType, &bet, &player_biggest_bet);

          // Envia para o apostador da rodada ou aposta
          sendPlay(&player, g_socket, betType, bet, player_biggest_bet, &result);

          // Atualiza o vetor de valores
          // TODO: updateChips(player);

          // Envia saldo atualizado
          sendNewChips(&player, g_socket, player_biggest_bet);

          // Passa o bastao
          passBaton(g_socket, player.playerID);
      } else {
          // Recebe a aposta e determina se vai ou não jogar
          receiveBet(&player, g_socket);

          // Se for jogar, faz a jogada
          receivePlay(&player, g_socket);

          // Recebe saldo atualizado do jogador ganhador
          receiveNewChips(&player, g_socket);

          // Recebe o bastão ou apenas repassa a mensagem
          message_t *msg_baton = receiveMessage(g_socket);
          receiveBaton(g_socket, msg_baton);
      }
  }		
	return 0;
}

/******** FUNÇÕES CONTROLE ********/
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

void pressToStart(){
  printf(" BEM VINDO AO YAHTZEE-EM-ANEL \n");
  printf("\nPressione \"s\" para iniciar...\n");
  char start;
  do{
    start = getc(stdin);
  }while (start != 'c');
}

void sendConf(player_t *player, game_socket_t *g_socket, int type){
  message_t *msg_conf = create_message(player->playerID, TYPE_CONF);
  msg_conf->result = type;
  message_t *msg_conf_response = sendAndWait(g_socket, msg_conf);
}

int receiveConf(player_t *player, game_socket_t *g_socket){
  message_t *msg_conf = receiveMessage(g_socket);
  int result = 1;
  if(msg_conf->type == TYPE_CONF){
    result = msg_conf->result;
  }

  sendMessage(g_socket, msg_conf);
  return result;
}

/* 1 -> DEFINE APOSTA */
void defineBet(player_t *player, game_socket_t *g_socket, int *betType, int *betValue, int *playerBet) {
    // Defino a jogada
    define_betType(player);
    message_t *msg_move = create_message(player->playerID, TYPE_BET);
    msg_move->bet_type = player->betType;
    msg_move->bet = player->betValue;
    msg_move->player = player->playerID;
  
    // Envia e espera o retorno com maior aposta
    message_t *msg_move_response = sendAndWait(g_socket, msg_move);

    printf("Maior aposta %d, jogador %d\n", msg_move_response->bet, msg_move_response->player);
    // Seta valores de retorno
    *betType = msg_move_response->bet_type;
    *betValue = msg_move_response->bet;
    *playerBet = msg_move_response->player;
}

void receiveBet(player_t *player, game_socket_t *g_socket){
  message_t *msg_move = receiveMessage(g_socket);

  if(msg_move->type == TYPE_BET){
    // Defino a minha jogada
    int result = define_betValue(player, msg_move->bet_type, msg_move->bet);
    if(result){
      // Edito a mensagem com um novo valor
      msg_move->bet_type = player->betType;
      msg_move->bet = player->betValue;
      msg_move->player = player->playerID;
    }
  }  

  // Mando a mensagem
  sendMessage(g_socket, msg_move);
}

/* 2 -> JOGA */
void sendPlay(player_t *player, game_socket_t *g_socket, int betType, int betValue, int playerBet, int *result){
  message_t *msg_play = create_message(player->playerID, TYPE_PLAY);
  msg_play->bet_type = betType;
  msg_play->bet = betValue;
  msg_play->player = playerBet;

  // Manda mensagem e espera receber o retorno
  message_t *msg_play_response = sendAndWait(g_socket, msg_play);

  // Se outro jogador jogou
  if(msg_play->player != player->playerID){
    *result = msg_play_response->result;
  } else {
    // Se ele mesmo joga
    *result = make_play(player);
  }
}

void receivePlay(player_t *player, game_socket_t *g_socket){
    // Espera a mensagem
    message_t *msg_play;
    do {
      msg_play = receiveMessage(g_socket);
    } while (msg_play->type != TYPE_PLAY);

    // Se eu sou o destino
    if(msg_play->player == player->playerID){
        // Faz a jogada
        int result = make_play(player);
        // Edita mensagem com resultado
        msg_play->result = result;
    }

    // Manda mensagem
    sendMessage(g_socket, msg_play);
}

/* 3 -> Atualiza saldo */
void sendNewChips(player_t *player, game_socket_t *g_socket, int playerBet){
  message_t *msg_update = create_message(player->playerID, TYPE_CHIPS);
  msg_update->player = playerBet;
  msg_update->result = player->playersChips[playerBet];
  message_t *msg_update_response = sendAndWait(g_socket, msg_update);
}

void receiveNewChips(player_t *player, game_socket_t *g_socket){
  // Espera o saldo novo
  message_t *msg_update = receiveMessage(g_socket);
  do {
    msg_update = receiveMessage(g_socket);
  } while (msg_update->type != TYPE_CHIPS);

  // Atualiza saldo se player válido
  int playerUpdate = msg_update->player;
  int value = msg_update->result;
  if(playerUpdate < 0 || playerUpdate >= 4){
    perror("Player inválido \n");
    return;
  }
  player->playersChips[playerUpdate] = value;

  // Manda mensagem
  sendMessage(g_socket, msg_update);
}