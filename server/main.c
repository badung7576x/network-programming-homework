#include "server.h"

int uid = 0;
int roomid = 0;
int client_count = 0;
int room_count = 0;

client_t *clients[MAX_CLIENTS];
room_t *rooms[MAX_ROOMS];
char *ip = "127.0.0.1";

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t room_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv){

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    pthread_t tid;

    if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	int port = atoi(argv[1]);

    /* Socket settings */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

     /* Bind */
    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Socket binding failed");
        return EXIT_FAILURE;
    }

    /* Listen */
    if (listen(listenfd, 10) < 0) {
        perror("Socket listening failed");
        return EXIT_FAILURE;
    }

     printf("<[ CHATAPP SERVER STARTED ]>\n");


    while (1) {
        socklen_t clilen = sizeof(cli_addr);
        connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

    if ((client_count + 1) == MAX_CLIENTS) {
            printf("<< max clients reached\n");
            close(connfd);
            continue;
        }

        /* Client settings */
        client_t *cli = (client_t *)malloc(sizeof(client_t));
        cli->addr = cli_addr;
        cli->connfd = connfd;
        cli->uid = uid++;
        cli -> currentRoomid = -1;
        char temp[64];
        sprintf(temp, "new user%d", cli->uid);
        strcpy(cli -> name, temp);
       
        /* Add client to the queue and fork thread */
        add_to_clients(cli);
        pthread_create(&tid, NULL, &handle_client, (void*)cli);

    }

}