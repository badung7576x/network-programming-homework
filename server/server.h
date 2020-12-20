#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#define MAX_CLIENTS 100
#define MAX_ROOMS 100
#define BUFFER_SZ 2048
#define PORT 3205

extern int uid;
extern int roomid;
extern int client_count;
extern int room_count;

/* Client structure */
typedef struct {
    struct sockaddr_in addr;    /* Client remote address */
    int connfd;                 /* Connection file descriptor */
    int uid;                    /* Client unique identifier */
    char name[32];              /* Client name */
    int currentRoomid;          /* Current room id of client */
} client_t;

/* Room structure */
typedef struct {
    int roomid;                 /* Room unique identifier */
    char name[32];              /* Room name */
    int usercount;              /* total user count that entered the room */
    int userids[20];            /* userids of users in room */
    int isPrivate;          
    char pass[32];
    int actualusers;            /* actual user count at that moment*/
    int roomRemoved;            /* 1 if room has no user */
} room_t;

extern client_t *clients[MAX_CLIENTS];
extern room_t *rooms[MAX_ROOMS];

extern pthread_mutex_t clients_mutex;
extern pthread_mutex_t room_mutex;


void send_message_sender(const char *s, int connfd);
void send_message_to_room(const char *s, int roomid, int senderid);
int find_room_id_by_name(char *str);
void add_to_rooms(room_t *room);
int check_room_name(char *str);
void add_to_clients(client_t *client);
void *handle_client(void *arg);
void list_rooms(client_t *client);