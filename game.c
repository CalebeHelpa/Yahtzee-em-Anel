#include "game.h"

void initialize_player(player_t *player, int playerId){
    player->playerID = playerId;
    player->nxtPlayerID = playerId == 3 ? 0 : playerId + 1;
    player->totalChips = INITCHIPS;
    player->betValue = 0;
    player->betType = -1;
}
