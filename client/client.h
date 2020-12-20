#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#define LENGTH 1024

extern volatile sig_atomic_t flag;
extern int clientSocket;

void * receive_message(void * sockID);
void catch_ctrl_c_and_exit(int sig);
void send_msg_handler();
void recv_msg_handler();