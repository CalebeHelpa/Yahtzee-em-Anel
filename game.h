#ifndef __GAME__
#define __GAME__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITCHIPS 10

typedef struct player_t
{
  int playerID;
  int nxtPlayerID;

  int totalChips;
  int betValue;
  int betType;

} player_t ;

void initialize_player(player_t *player, int playerId);

#endif