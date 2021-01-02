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
#define LENGTH 4096

extern volatile sig_atomic_t flag;
extern int clientSocket;

void * receive_message(void * sockID);
void send_msg_handler();
void recv_msg_handler();
void catch_ctrl_c_and_exit(int sig);

int connect_to_server(char *ip, int port);
void create_room(char *username, char *room_name);
void join_room(char *username, char *room_name);
void get_rooms();
void get_clients_in_room();
void send_message_to_all(char *message);
void send_message_to_one(char *message, char *username);
void exit_room();