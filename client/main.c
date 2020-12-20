#include "client.h"

volatile sig_atomic_t flag = 0;
int clientSocket;
char *ip = "127.0.0.1";

int main(int argc, char **argv){

	struct sockaddr_in serverAddr;

    if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	int port = atoi(argv[1]);

    signal(SIGINT, catch_ctrl_c_and_exit);

	/* Socket settings */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(ip);

    int err = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (err == -1) {
		printf("ERROR: connect\n");
		return EXIT_FAILURE;
	}

	pthread_t recv_msg_thread;
  	if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

	pthread_t send_msg_thread;
  	
	if(pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0){
		printf("ERROR: pthread\n");
    	return EXIT_FAILURE;
	}

	while (1){
		if(flag){
			printf("\nBye\n");
			break;
    	}
	}

	close(clientSocket);

	return EXIT_SUCCESS;
}