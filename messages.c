#include "messages.h"
int GAME_PORTS[] = {8080, 8081, 8082, 8083};

int receiveMessage(game_socket_t *g_socket, char *buffer){
    int len = sizeof(g_socket->prevaddr);
    int n = recvfrom(g_socket->sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &(g_socket->prevaddr), &len);

    return n;
}

int sendMessage(game_socket_t *g_socket, char *buffer){
    sendto(g_socket->sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &(g_socket->nextaddr), sizeof(g_socket->nextaddr));
    return 1;
}

int getPort(int id){
    return GAME_PORTS[id];
}

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

    return g_socket;
}