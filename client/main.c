#include "client.h"

volatile sig_atomic_t flag = 0;
int clientSocket;

int main(int argc, char **argv){

    if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}
	char *ip = "127.0.0.1";
	int port = atoi(argv[1]);

	return connect_to_server(ip, port);

}