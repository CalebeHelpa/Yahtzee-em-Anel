#include "game.h"

char* itos_bet(int bet){
    switch(bet){
    case PAR:
        return("Par");
    case TRIO:
        return("Trio");
    case DOISPARES:
        return("Dois pares");
    case FULLHOUSE:
        return("Full house");
    case SEQBAIXA:
        return("Sequencia baixa");
    case SEQALTA:
        return("Sequencia alta");
    case QUADRA:
        return("Quadra");
    case QUINTETO:
        return("Quinteto");
    default:
        return("ERRO: APOSTA INVALIDA SELECIONADA");
        break;
    }

}

void initialize_player(player_t *player, int playerId){
    player->playerID = playerId;
    player->nxtPlayerID = playerId == (TOTALPLAYERS-1) ? 0 : playerId + 1;

    if(player->playerID == 0)
        player->baton = 1;

    for(int i = 0; i <= (TOTALPLAYERS-1); i++)
        player->playersChips[i] = INITCHIPS;
    
    player->betType = NULO;
    player->betValue = 0;

    player->automate = 0;
}

void define_betType(player_t *player){

    fprintf(stdout, "Defina o tipo de aposta selecionando o numero correspondente:\n");
    fprintf(stdout, "Um par ---------------------------- 1\n");
    fprintf(stdout, "Um trio --------------------------- 2\n");
    fprintf(stdout, "Dois pares ------------------------ 3\n");
    fprintf(stdout, "Full house (Um par e um trio) ----- 4\n");
    fprintf(stdout, "Sequencia baixa ------------------- 5\n");
    fprintf(stdout, "Sequencia alta -------------------- 6\n");
    fprintf(stdout, "Quadra ---------------------------- 7\n");
    fprintf(stdout, "Quinteto -------------------------- 8\n");


    int type = getc(stdin) - 48;
    getc(stdin);        // Remove new line do buffer de entrada
    while(type < 1 || type > 8){
        fprintf(stdout, "Selecione uma entrada valida!\n");
        type = getc(stdin) - 48;
        getc(stdin);    // Remove new line do buffer de entrada
    }
    
    player->betType = type;
    player->betValue = 1;
}

int define_betValue(player_t *player, int betType, int betValue){
    
    player->betType = NULO;
    player->betValue = 0;

    fprintf(stdout, "A aposta atual é: %s pelo valor %i\n", itos_bet(betType), betValue);
    fprintf(stdout, "Digite o valor que deseja apostar, caso nao queira digite um valor menor que a aposta atual\n");
    
    int value;
    scanf("%d", &value);
    if(value <= betValue){
        fprintf(stdout, "Valor da aposta mantido, passando para o proximo jogador...\n");
        return 0;
    }

    player->betType = betType;
    player->betValue = value;
    return 1;
}

void throw_dices(int **dices){
    srand(time(NULL));
    for(int i = 0; i < DICES; i++)
        if(dices[0][i] == 0)
            dices[1][i] = (rand() % 5) + 1;
}

void freeze_dices(int **dices){
    for(int i = 0; i < DICES; i++)
        fprintf(stdout, "%d ", dices[1][i]);
    fprintf(stdout, "\n");

    
}

int make_play(player_t *player){
    
    player->playersChips[player->playerID] -= player->betValue;

    int dices[1][DICES-1];
    for(int i = 0; i < DICES; i++){
        dices[0][i] = 0;
        dices[1][i] = 0;
    }
    

    int numPlays;
    for(numPlays = 0; numPlays < 3; numPlays++){ 
        throw_dices(dices);
        freeze_dices(dices);
    }
    
    
}
