#include "messages.h"
int GAME_PORTS[] = {8080, 8081, 8082, 8083};

/*********** UTIL ***********/
int getPort(int id){
    return GAME_PORTS[id];
}

void str_append(char *str, int *size, char value){
    str[*size] = value;
    (*size)++;
}

int convertMsgToStr(message_t *msg, char *buffer){
    int size = 0;
 
    str_append(buffer, &size, START_MARKER);
    str_append(buffer, &size, msg->type);
    str_append(buffer, &size, msg->origin);

    // TODO: Alterar para ter tamanho fixo
    str_append(buffer, &size, msg->bet_type + '0');
    str_append(buffer, &size, msg->bet + '0');
    str_append(buffer, &size, msg->result + '0');

    // Calcula erro
    char erro = '0';
    for(int i = 3; i < size; i++){
        erro ^= buffer[i];
    }

    str_append(buffer, &size, erro);

    return size;
}

message_t *convertStrToMsg(char *buffer, int size){
    printf("Recebido: %d\n", size);

    for(int i = 0; i < size; i++){
        printf("%d |", buffer[i]);
    }
    printf("\n");

    char marker = buffer[0];
    if(marker != START_MARKER){
        perror("Marcador inicial não corresponde!\n");
        printf(">%x\n", marker);
    }

    char type = buffer[1];
    message_t *msg = create_message(0, type);
    if(!msg){
        return NULL;
    }

    msg->origin = (int) buffer[2] - 48;

    msg->bet_type = (int) buffer[3] - 48;
    msg->bet = (int) buffer[4] - 48;
    msg->result = (int) buffer[5] - 48;

    char erro = '0';
    for(int i = 3; i < (size-1); i++){
        erro ^= buffer[i];
    }

    char error_rcv = buffer[size-1];
    if(error_rcv != erro){
        perror("Dados devem estar incorretos\n");
    }

    return msg;
}

/*********** MENSAGEM ***********/
message_t *receiveMessage(game_socket_t *g_socket){
    char buffer[MAXLINE];
    int len = sizeof(g_socket->prevaddr);
    int n = recvfrom(g_socket->sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &(g_socket->prevaddr), &len);

    return convertStrToMsg(buffer, n);
}

int sendMessage(game_socket_t *g_socket, message_t *msg){
    // Converte a mensagem
    char *buffer;
    buffer = malloc(sizeof(char) * 156);
    if(!buffer){
        perror("Não foi possível alocar espaço para o buffer\n");
        return -1;
    }

    int size = convertMsgToStr(msg, buffer);
    if(size < 0){
        return -1;
    }

    for(int i = 0; i < size; i++){
        printf("%d |", buffer[i]);
    }
    printf("\n");

    printf("Enviando: %d\n", size);
    fflush(stdout);
    // Envia mensagem
    sendto(g_socket->sockfd, (const char *)buffer, size, MSG_CONFIRM, (const struct sockaddr *) &(g_socket->nextaddr), sizeof(g_socket->nextaddr));
    return 1;
}

/*********** BASTAO ***********/
void passBaton(game_socket_t *g_socket){
    // Envia mensagem para o próximo com o bastão
    message_t *msg = create_message(g_socket->myPort, TYPE_BATON);
    msg->result = 0;
    sendMessage(g_socket, msg);

    // Espera confirmação
    message_t *result = receiveMessage(g_socket);
    
    // Seta bastão como 0 se a resposta for valida
    if(result != NULL && result->result == 1){
        g_socket->baton = 0;
    }
}

void receiveBaton(game_socket_t *g_socket, message_t *msg){
    // Verifica se o bastao é pra ele
    // Se for seta o valor
    if(msg->result == 0){
        g_socket->baton = 1;
        msg->result = 1;
    }

    // Passa a mensagem
    sendMessage(g_socket, msg);
}

message_t* create_message (int origin, char type) {
    message_t* msg = malloc(sizeof(message_t));
    if(!msg){
        perror("Não foi possível alocar a mensagem !\n");
        return NULL;
    }

    msg->origin = origin;
    msg->type = type;

    return msg;
}

/*********** SOCKET ***********/

game_socket_t* create_game_socket (int myPort, int nxtPort){
    
    game_socket_t* g_socket = malloc(sizeof(game_socket_t));
	if(!g_socket){
        perror("Erro ao alocar game socket !\n");
		return NULL;
    }

    // Preenche informação dos ports
    g_socket->myPort = myPort;
    g_socket->nxtPlayerPort = nxtPort;

    // Cria o descritor do socket
	g_socket->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_socket->sockfd < 0){
		perror("Erro ao criar o socket!\n");
		return NULL;
	}
    
    // Zera espaço de memória dos endereços
	memset(&(g_socket->myaddr), 0, sizeof(g_socket->myaddr));
	memset(&(g_socket->nextaddr), 0, sizeof(g_socket->nextaddr));
	memset(&(g_socket->prevaddr), 0, sizeof(g_socket->prevaddr));
		
	// Preenche os dados do endereço do player
	g_socket->myaddr.sin_family = AF_INET; // IPv4
	g_socket->myaddr.sin_addr.s_addr = INADDR_ANY;
	g_socket->myaddr.sin_port = htons(myPort);
		
	// Associa com o socket
	if (bind(g_socket->sockfd, (const struct sockaddr *)&(g_socket->myaddr), sizeof(g_socket->myaddr)) < 0) {
		perror("Não foi possível conectar o socket ao endereço!\n");
		exit(EXIT_FAILURE);
	}

    g_socket->nextaddr.sin_family = AF_INET; 
    g_socket->nextaddr.sin_port = htons(nxtPort); 
    g_socket->nextaddr.sin_addr.s_addr = INADDR_ANY;

    // Seta o bastao para o primeiro jogador
    if(myPort == GAME_PORTS[0]){
        g_socket->baton = 1;
    } else {
        g_socket->baton = 0;
    }

    return g_socket;
}