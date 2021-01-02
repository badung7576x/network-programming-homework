#include "client.h"
#include "../common/common.h"

void catch_ctrl_c_and_exit(int sig) {
    flag = 1;
}

void send_msg_handler() {
  	char message[LENGTH] = {};
  	while(1) {
		fgets(message, LENGTH, stdin);
		strip_newline(message);
		if(message[0]=='-'){
			send(clientSocket,message,LENGTH,0);
			if(!strcmp(message, "-exit")){
				break;
			}
		}
		bzero(message, LENGTH);
	}
  	catch_ctrl_c_and_exit(2);
}

void recv_msg_handler() {
	char message[LENGTH] = {};
	int receive;
  	while (1) {
		receive = recv(clientSocket, message, LENGTH, 0);
		if (receive > 0) {
			printf("%s\n", message);
		} else if (receive == 0) {
			break;
		} else {
			
		}
		bzero(message, LENGTH);
  	}
}

int connect_to_server(char *ip, int port) {
	struct sockaddr_in server_addr;

    signal(SIGINT, catch_ctrl_c_and_exit);

	/* Socket settings */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);

    int err = connect(clientSocket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (err == -1) {
		printf("ERROR: connect\n");
		return EXIT_FAILURE;
	}

	pthread_t recv_msg_thread;
  	if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
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

void create_room(char *username, char *room_name){
	char message[LENGTH] = {};
	sprintf(message, "[%s][%s][%s][%s]\n", username, "/create", room_name, "---");
	send(clientSocket, message, LENGTH, 0);
	bzero(message, LENGTH);

  	catch_ctrl_c_and_exit(2);
}


void join_room(char *username, char *room_name){
	char message[LENGTH] = {};
	sprintf(message, "[%s][%s][%s][%s]\n", username, "/join", room_name, "---");
	send(clientSocket, message, LENGTH, 0);
	bzero(message, LENGTH);

  	catch_ctrl_c_and_exit(2);
}




