#include "client.h"
#include "../common/common.h"

void * receive_message(void * sockID){

	int clientSocket = *((int *) sockID);

	while(1){
		char data[3000];
		int read = recv(clientSocket,data,3000,0);
		data[read] = '\0';
		printf("%s\n",data);
	}

}

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
		memset(message, 0, sizeof(message));
  	}
}