// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <getopt.h>
	
#define PORT	 8080
#define MAXLINE 1024
#define INITCHIPS 10

int PORTS[] = {8080, 8081, 8082, 8083};

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

void initialize_player(player_t player, int playerId){
  
  player.playerID = playerId;
  player.nxtPlayerID = playerId == 3 ? 0 : playerId + 1;
  // int previousPlayerId = playerId == 3 ? 0 : playerId + 1;

  player.totalChips = INITCHIPS;
  player.betValue = 0;
  player.betType = NULL;
}


// Driver code
int main(int argc, char ** argv) { 
  // Configurações da máquina
  player_t player;
  int playerId = get_player_id(argc, argv);
  initialize_player(player, playerId);

  int port = PORTS[playerId];
  int next_port = PORTS[player.nxtPlayerID];
  // int next_port = PORTS[nextPlayerId];

  // Inicia
	int sockfd;
	char buffer[MAXLINE];

	struct sockaddr_in myaddr, nextaddr, prevaddr;
		
	// Creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
		
	memset(&myaddr, 0, sizeof(myaddr));
	memset(&nextaddr, 0, sizeof(nextaddr));
	memset(&prevaddr, 0, sizeof(prevaddr));
		
	// Filling server information
	myaddr.sin_family = AF_INET; // IPv4
	myaddr.sin_addr.s_addr = INADDR_ANY;
	myaddr.sin_port = htons(port);
		
	// Bind the socket with the server address
	if (bind(sockfd, (const struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

  nextaddr.sin_family = AF_INET; 
  nextaddr.sin_port = htons(next_port); 
  nextaddr.sin_addr.s_addr = INADDR_ANY; 
        	
	int len, n;
	
	len = sizeof(prevaddr); //len is value/result

  player_t player;
  initialize_player(player, playerId);
  
  int cicle = 0;	
  while(1){
    if(playerId > 0 || (playerId == 0 && cicle > 0)){
      // recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
      n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &prevaddr, &len);
      buffer[n] = '\0';
      cicle = atoi(buffer);
      printf("Recebido: %d\n", cicle);
    }

    cicle++;
    char msg[MAXLINE];
    sprintf(msg, "%d", cicle);

    sendto(sockfd, (const char *)msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *) &nextaddr, sizeof(nextaddr)); 
    // sendto(sockfd, (const char *)msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
    printf("Enviado: %s\n", msg);
    printf("-----\n");
    sleep(2);
  }		
	return 0;
}