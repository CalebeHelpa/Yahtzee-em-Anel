
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

    for(int i = 0; i < TOTALPLAYERS; i++)
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


    int type;
    scanf("%d", &type);
    while(type < 1 || type > 8){
        fprintf(stdout, "Selecione uma entrada valida!\n");
        scanf("%d", &type);
    }
    
    player->betType = type;
    player->betValue = 1;
}

int define_betValue(player_t *player, int betType, int betValue){
    
    player->betType = NULO;
    player->betValue = 0;

    fprintf(stdout, "A aposta atual é: %s pelo valor %i\n", itos_bet(betType), betValue);
    fprintf(stdout, "O seu saldo é: %i\n", player->playersChips[player->playerID]);
    fprintf(stdout, "Digite o valor que deseja apostar, caso nao queira digite um valor menor que a aposta atual\n");
    
    int value;
    scanf("%d", &value);
    if(value <= betValue || value > player->playersChips[player->playerID]){
        fprintf(stdout, "Valor da aposta mantido, passando para o proximo jogador...\n");
        return 0;
    }

    player->betType = betType;
    player->betValue = value;
    return 1;
}

void show_bloqDices(int dices[2][NUMDICES]){
    fprintf(stdout, "Os dados bloqueados sao:\n");
    for(int i = 0; i < NUMDICES; i++)
        if(dices[0][i] == 1)
            fprintf(stdout, "%d ", dices[1][i]);
    fprintf(stdout, "\n");
}

void show_dices(int dices[2][NUMDICES]){
    fprintf(stdout, "Os resultados sao:\n");
    for(int i = 0; i < NUMDICES; i++)
        fprintf(stdout, "%d ", dices[1][i]);
    fprintf(stdout, "\n");
}

void throw_dices(int dices[2][NUMDICES]){
    fprintf(stdout, "Jogando os dados...\n");
    srand(time(NULL));
    for(int i = 0; i < NUMDICES; i++)
        if(dices[0][i] == 0)
            dices[1][i] = (rand() % 5) + 1;
    show_dices(dices);
}

void freeze_dices(int dices[2][NUMDICES]){
    fprintf(stdout, "Insira uma sequencia de zeros e uns, separados por enter, para os dados que deseja bloquear, respectivamente\n");
    fprintf(stdout, "(Um para bloquear, zero para rolar novamente)\n");
    fprintf(stdout, ">>> Atencao! Uma vez o dado bloqueado ele nao pode ser desbloqueado!\n");

    int buff = 0;
    for(int i = 0; i < NUMDICES; i++){
        scanf("%d", &buff);
        if(dices[0][i] == 0)
            dices[0][i] = buff;
    }

    show_bloqDices(dices);

}

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
// Function to perform Selection Sort
void selectionSort(int arr[], int n)
{
    int i, j, min_idx;
 
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {
 
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
 
        // Swap the found minimum element
        // with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

int check_par(player_t *player, int dices[2][NUMDICES]){
    int i = 0;
    int j = 1;
    int find = 0;
    while(i < NUMDICES-1 && j < NUMDICES && find == 0){
        if(dices[1][i] == dices[1][j])
            find = 1;
        i++;
        j++;
    }
    if(find)
        player->playersChips[player->playerID] += RECPAR;
        
    return player->playersChips[player->playerID];
}

int check_trio(player_t *player, int dices[2][NUMDICES]){
    int i = 0;
    int j = 1;
    int k = 2;
    int find = 0;
    while(i < NUMDICES-2 && j < NUMDICES-1 && k < NUMDICES && find == 0){
        if((dices[1][i] == dices[1][j]) && (dices[1][j] == dices[1][k]))
            find = 1;
        i++;
        j++;
        k++;
    }
    if(find)
        player->playersChips[player->playerID] += RECTRIO;
        
    return player->playersChips[player->playerID];
}

int check_doispares(player_t *player, int dices[2][NUMDICES]){
    int i = 0;
    int j = 1;
    int k = 2;
    int l = 3;
    int find = 0;
    while(i < NUMDICES-3 && j < NUMDICES-2 && k < NUMDICES-1 && l < NUMDICES && find == 0){
        if((dices[1][i] == dices[1][j]) && (dices[1][k] == dices[1][l]))
            find = 1;
        i++;
        j++;
        k++;
        l++;
    }
    if(find)
        player->playersChips[player->playerID] += RECDOISPARES;
        
    return player->playersChips[player->playerID];
}

int check_fullhouse(player_t *player, int dices[2][NUMDICES]){
    int i = 0;
    int j = 1;
    int k = 2;
    int l = 3;
    int m = 4;
    int find = 0;

    if((dices[1][i] == dices[1][j]) && ((dices[1][k] == dices[1][l]) && (dices[1][l] == dices[1][m])))
        find = 1;
    else if ((dices[1][l] == dices[1][m]) && ((dices[1][i] == dices[1][j]) && (dices[1][j] == dices[1][k])))
        find = 1;

    if(find)
        player->playersChips[player->playerID] += RECFULLHOUSE;
        
    return player->playersChips[player->playerID];
}

int check_seqbaixa(player_t *player, int dices[2][NUMDICES]){
    int find = 0;
    if((dices[1][0] == 1) && (dices[1][1] == 2) && (dices[1][2] == 3) && (dices[1][3] == 4) && (dices[1][4] == 5))
        find = 1;

    if(find)
        player->playersChips[player->playerID] += RECSEQBAIXA;
        
    return player->playersChips[player->playerID];
}

int check_seqalta(player_t *player, int dices[2][NUMDICES]){
    int find = 0;
    if((dices[1][0] == 2) && (dices[1][1] == 3) && (dices[1][2] == 4) && (dices[1][3] == 5) && (dices[1][4] == 6))
        find = 1;

    if(find)
        player->playersChips[player->playerID] += RECSEQALTA;
        
    return player->playersChips[player->playerID];
}

int check_quadra(player_t *player, int dices[2][NUMDICES]){
    int i = 0;
    int j = 1;
    int k = 2;
    int l = 3;
    int find = 0;
    while(i < NUMDICES-3 && j < NUMDICES-2 && k < NUMDICES-1 && l < NUMDICES && find == 0){
        if((dices[1][i] == dices[1][j]) && (dices[1][j] == dices[1][k]) && (dices[1][k] == dices[1][l]))
            find = 1;
        i++;
        j++;
        k++;
        l++;
    }
    if(find)
        player->playersChips[player->playerID] += RECQUADRA;
        
    return player->playersChips[player->playerID];
}

int check_quinteto(player_t *player, int dices[2][NUMDICES]){
    int i = 0;
    int j = 1;
    int k = 2;
    int l = 3;
    int m = 4;
    int find = 0;
    while(i < NUMDICES-4 && j < NUMDICES-3 && k < NUMDICES-2 && l < NUMDICES-1 && m < NUMDICES && find == 0){
        if((dices[1][i] == dices[1][j]) && (dices[1][j] == dices[1][k]) && (dices[1][k] == dices[1][l]) && (dices[1][l] == dices[1][m]))
            find = 1;
        i++;
        j++;
        k++;
        l++;
        m++;
    }
    if(find)
        player->playersChips[player->playerID] += RECQUINTETO;

    return player->playersChips[player->playerID];
}

int define_result(player_t *player, int dices[2][NUMDICES]){
    selectionSort(dices[1], NUMDICES);
    switch(player->betType){
    case PAR:
        return check_par(player, dices);
    case TRIO:
        return check_trio(player, dices);
    case DOISPARES:
        return check_doispares(player, dices);
    case FULLHOUSE:
        return check_fullhouse(player, dices);
    case SEQBAIXA:
        return check_seqbaixa(player, dices);
    case SEQALTA:
        return check_seqalta(player, dices);
    case QUADRA:
        return check_quadra(player, dices);
    case QUINTETO:
        return check_quinteto(player, dices);
    default:
        return(-1);
        break;
    }
}

int make_play(player_t *player){
    
    fprintf(stdout, "Pressione qualquer tecla para jogar os dados\n");

    player->playersChips[player->playerID] -= player->betValue;

    int dices[2][NUMDICES];
    for(int i = 0; i < NUMDICES; i++){
        dices[0][i] = 0;
        dices[1][i] = 0;
        fprintf(stdout, "%d\n", i);
    }

    int numPlays = 0;
    int playerStop = 0;

    while(numPlays < 3 && playerStop == 0){
        throw_dices(dices);
        freeze_dices(dices);
        fprintf(stdout, "Se deseja jogar os dados novamente pressione 1, senao pressione 0\n");
        scanf("%d", &playerStop);
        numPlays++;
    }

    return define_result(player, dices);
}

// Retorna 1 caso seja o final do jogo e 0 caso contrario
int end_game(player_t *player){
    for(int i = 0; i < TOTALPLAYERS; i++)
        if(player->playersChips[i] <= 0)
            return 1;
    return 0;
}