#ifndef __GAME__
#define __GAME__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INITCHIPS 10
#define TOTALPLAYERS 4
#define NUMDICES 5

#define NULO -1
#define PAR 1
#define TRIO 2
#define DOISPARES 3
#define FULLHOUSE 4
#define SEQBAIXA 5
#define SEQALTA 6
#define QUADRA 7
#define QUINTETO 8

#define RECPAR 2
#define RECTRIO 3
#define RECDOISPARES 4
#define RECFULLHOUSE 5
#define RECSEQBAIXA 7
#define RECSEQALTA 7
#define RECQUADRA 10
#define RECQUINTETO 15

typedef struct player_t {
  int playerID;                     // ID do jogador
  int nxtPlayerID;                  // ID do proximo jogador

  int playersChips[TOTALPLAYERS];   // Vetor para monitorar fichas dos jogadores atraves do ID 
  
  int betType;                      // Tipo da aposta
  int betValue;                     // Valor apostado
  
  int automate;                     // 1 se e jogador automatico, 0 caso contrario

} player_t;

void initialize_player(player_t *player, int playerId);

void define_betType(player_t *player);

int define_betValue(player_t *player, int betType, int betValue); // Retorna 1 caso o player tenha apostado e 0 caso contrario

int make_play(player_t *player); // Retorna 1 em caso de sucesso e 0 em caso de falha

int end_game(player_t *player); // Retorna 1 caso seja o final dojogo e 0 caso contrario

#endif