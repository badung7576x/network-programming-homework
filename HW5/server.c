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
#include "file.h"
#define LOCALHOST "127.0.0.1"
#define BUFFER 1024
#define FILENAME "nguoidung.txt"

extern Account *head;
void die(char *s);


int main(int argc, char **argv) {
    
    // Init variable
    int sockfd = 0, connfd, step = 1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t cli_addr_size = sizeof(client_addr);
    char buffer[BUFFER], data[BUFFER];
    char username[MAXCHAR], password[MAXCHAR], newpass[MAXCHAR];
    char logFile[2048];
    
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return -1;
    }
    if(readfile(FILENAME) == 0) return -1;

    int port = atoi(argv[1]);
    

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        die("Error socket");
    };
    
    // Set server address, port
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(LOCALHOST);

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))){
        die("bind");
    }
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        die("listen");
    } 
    // Accept the data packet from client and verification  
    if ((connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cli_addr_size)) < 0) { 
        die("accept");
    } 
    printf("Server started.\n");

    // Keep listening for data
    while(1) {
        printf("Waiting for data...\n");
		fflush(stdout);
        
        // Try to receive data
        if ((read(connfd, buffer, sizeof(buffer))) < 0) {
            die("read()");
        }
        getInput(buffer, data);
        if(strcmp(data, "bye") == 0) {
            step = 4;
        }
            
        // Print details of the client/peer and the data received
        printf("Data received: %s\n" , data);

        switch(step) {
            case 1:
                if(hasAccount(data) == FALSE) {
                    strcpy(buffer, "Wrong account");
                } else {
                    strcpy(username, data);
                    strcpy(buffer, "Insert password");
                    step++;
                }
                break;
            case 2:
                strcpy(password, data);
                int status = signIn(username, password);
                writefile(FILENAME);
                if(status == LOGIN_SUCCESS) {
                    sprintf(logFile, "%s_logfile.txt", username);
                    writeLog(logFile, username);
                    strcpy(buffer, "OK");
                    step++;
                } else if (status == LOGIN_FAILURE) {
                    strcpy(buffer, "Not OK");
                } else if (status == LOGIN_IDLE) {
                    strcpy(buffer, "Account not ready");
                    step = 1;
                } else {
                    strcpy(buffer, "Account is blocked");
                    step = 1;
                }
                break;
            case 3:    
                strcpy(newpass, data);
                if(changePassword(username, newpass) == TRUE) {
                    writefile(FILENAME);
                    strcpy(buffer, newpass);
                    encodePass(newpass, buffer);
                } else {
                    strcpy(buffer, "Error");
                };
                break;
            case 4:         
                strcpy(buffer, "Goobye ");
                strcat(buffer, username); 
                strcpy(username, "");
                strcpy(password, "");
                step = 1;
                break;
        }
        // Send data to client
        if ((write(connfd, buffer, sizeof(buffer)))  < 0){
            die("sendto()");
        }
    }

    close(sockfd);
    close(connfd);
    return 0;
}

void die(char *s)
{
	perror(s);
	exit(1);
}