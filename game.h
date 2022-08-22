#ifndef __GAME__
#define __GAME__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITCHIPS 10
#define TOTALPLAYERS 4
#define DICES 5

#define NULO -1
#define PAR 1
#define TRIO 2
#define DOISPARES 3
#define FULLHOUSE 4
#define SEQBAIXA 5
#define SEQALTA 6
#define QUADRA 7
#define QUINTETO 8

typedef struct player_t {
  int playerID;                     // ID do jogador
  int nxtPlayerID;                  // ID do proximo jogador

  int baton;                        // 1 se possui o bastao, 0 caso contrario
  int playersChips[TOTALPLAYERS-1]; // Vetor para monitorar fichas dos jogadores atraves do ID 
  
  int betType;                      // Tipo da aposta
  int betValue;                     // Valor apostado
  
  int automate;                     // 1 se e jogador automatico, 0 caso contrario

} player_t;

void initialize_player(player_t *player, int playerId);

void define_betType(player_t *player);

int define_betValue(player_t *player, int betType, int betValue); // Retorna 1 caso o player tenha apostado e 0 caso contrario

int make_play(player_t *player); // Retorna 1 em caso de sucesso e 0 em caso de falha

#endif