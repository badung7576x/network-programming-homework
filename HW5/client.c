#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "account.h"
#define MAXCHAR 100
#define BUFFER 1024

void die(char *s);

int main(int argc, char **argv) {
    
    char ip[MAXCHAR];
    int sockfd = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER], data[BUFFER];

    if (argc < 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        return -1;
    }
    
    strcpy(ip, argv[1]);
    int port = atoi(argv[2]);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        die("Error socket");
    };
    memset(&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(ip);

    // connect the client socket to server socket 
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0) { 
        die("Connect");
    } 

    while(1) {
        fgets(data, BUFFER, stdin);
        if(getInput(data, buffer) == FALSE) {
            die("Exit");
        }
        //send the message
		if (write(sockfd, buffer, sizeof(buffer)) < 0) {
			die("write()");
		}

        // Try to receive some data, this is a blocking call
        if (read(sockfd, buffer, sizeof(buffer)) < 0) {
            die("read()");
        }
        if(getInput(buffer, data) == FALSE) {
            die("The received data is corrupted!");
        }
        printf("%s\n", data);
    }

    close(sockfd);
    return 0;
}


void die(char *s)
{
	perror(s);
	exit(1);
}