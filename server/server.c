#include "server.h"
#include "../common/common.h"


/* Send message to sender */
void send_message_sender(const char *s, int connfd){
    if (write(connfd, s, strlen(s)) < 0) {
        perror("Error at send self message");
        exit(-1);
    }
}

/*
Sends message to room, except the sender.
*/
void send_message_to_room(const char *s, int roomid, int senderid){

    for(int i = 0;i<rooms[roomid]->usercount;i++){
        if(clients[rooms[roomid]->userids[i]]->currentRoomid == roomid && clients[rooms[roomid]->userids[i]]->uid != senderid ){
            if (write(clients[rooms[roomid]->userids[i]]->connfd, s, strlen(s)) < 0) {
                perror("Error at send room message");
                exit(-1);
            }
        }
    }
}

/*
Finds room id by its name.
*/
int find_room_id_by_name(char *str){

    for(int i = 0; i<room_count; i++){
        if(!strcmp(rooms[i]->name, str) && rooms[i]->roomRemoved!=1){
            return rooms[i]->roomid;
        }
    }
    
    return -1;
}

/*
Add new room to rooms
*/
void add_to_rooms(room_t *room){
    pthread_mutex_lock(&room_mutex);

    rooms[room_count] = room;
    if(room->isPrivate == 0 ){
        printf("Room created with name %s by %s \n",room->name, clients[room->userids[0]]->name);
    } 
    else {
        printf("Room created with name %s and password = %s by %s \n",room->name, room->pass, clients[room->userids[0]]->name);
    }
    
    clients[room->userids[0]]-> currentRoomid = room->roomid;
    send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", clients[room->userids[0]]->connfd);
    send_message_sender("You created and entered a room.", clients[room->userids[0]]->connfd);
    send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", clients[room->userids[0]]->connfd);
    send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ", clients[room->userids[0]]->connfd);
    send_message_sender(room->name, clients[room->userids[0]]->connfd);
    send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  \n", clients[room->userids[0]]->connfd);
    room_count++;
    pthread_mutex_unlock(&room_mutex);
}

/*
Checks if room name is available.
*/
int check_room_name(char *str){
    for (int i = 0; i < room_count; i++) {
        if(!strcmp(str, rooms[i]->name) && rooms[i]->roomRemoved != 1){
            return 0;
        }
    }
    return 1;
}

/*
List the rooms and its users.
*/
void list_rooms(client_t *client) {
    send_message_sender("  ~~~~~~  ~~~~~~  Room List  ~~~~~~  ~~~~~~ \n", client->connfd);
    for(int i = 0; i<room_count; i++){
        if(rooms[i]->actualusers>0){
            if(rooms[i]->isPrivate){
                send_message_sender("  ~~~~~~  ||", client->connfd);
                send_message_sender(rooms[i]->name, client->connfd);
                send_message_sender(" (Private Room)||   ~~~~~~", client->connfd);
                send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", client->connfd);
            } else {
                send_message_sender("  ~~~~~~  ||", client->connfd);
                send_message_sender(rooms[i]->name, client->connfd);
                send_message_sender("||  ~~~~~~ \n", client->connfd);
                send_message_sender("Users in Room : ", client->connfd);
                for(int a = 0; a<rooms[i]->usercount;a++){
                    if(clients[rooms[i]->userids[a]]->currentRoomid == rooms[i]->roomid){
                        send_message_sender(clients[rooms[i]->userids[a]]->name, client->connfd);
                        send_message_sender(", ", client->connfd);
                    }  
                }
                send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", client->connfd);
            }
        }
    }
}


/*
Adds a new client to clients.
*/
void add_to_clients(client_t *client){
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (!clients[i]) {
            clients[i] = client;
            break;
        }
    }
    
    pthread_mutex_unlock(&clients_mutex);

}

/*
All the operations of users and chatting system is done in this method.
*/
void *handle_client(void *arg) {
    char buff_in[BUFFER_SZ / 2];
    int rlen;

    char helpString[1024] = "-nick nickname: Choose a new nickname for yourself. \n";
    strcat(helpString, "-list: Lists the currently available rooms with the name of the customers in it. If the room is private, no details given. \n");
    strcat(helpString, "-create room_name: Creates a new specified room. Not more than one room with the same name. \n");
    strcat(helpString,"-pcreate room_name password: Creates a new specified private room. This type of room has been protected with password.\n");
    strcat(helpString,"-enter room_name password(if private): Enter to the specified room. If the room is private, the client must know the password for enter. \n" );
    strcat(helpString,"-quit: Quit from the room that you are in. You come back to the common area. \n");
    strcat(helpString,"-msg message_body: Sends a message to room that you are in. \n" );
    strcat(helpString,"-whoami: Shows your own nickname information.\n");
    strcat(helpString,"-exit: Exit the program.\n");
    strcat(helpString,"-help: To see this text again.\n");

    client_count++;
    client_t *cli = (client_t *)arg;

    printf("Accepted \" %s \" referenced by id %d\n",cli->name, cli->uid);

    send_message_sender("  ~~~~~~  Welcome to ChatApp  ~~~~~~ \n", cli->connfd);
    send_message_sender(helpString, cli->connfd);
    send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n \n \n \n", cli->connfd);
    send_message_sender(" Please pick a new username with \"-nick nickname\" command. \n", cli->connfd);

    while ((rlen = read(cli->connfd, buff_in, sizeof(buff_in) - 1)) > 0) {
        buff_in[rlen] = '\0';
        strip_newline(buff_in);

        if (!strlen(buff_in)) continue;

        if (buff_in[0] == '-') {
            char *first, *second, *third;
            first = strtok(buff_in," ");
            if(!strcmp(first, "-msg")){
                second = strtok(NULL," ");
                if(cli->currentRoomid != -1){
                    char messagetoRoom[300], messagetoSelf[300];
                    strcpy(messagetoSelf, "  |");
                    strcpy(messagetoRoom ,cli->name);
                    strcat(messagetoRoom, " --> |");
                    while(second != NULL){
                        strcat(messagetoRoom, " ");
                        strcat(messagetoRoom, second);

                        strcat(messagetoSelf, " ");
                        strcat(messagetoSelf, second);
                        second = strtok(NULL, " ");
                    }
                    
                    strcat(messagetoRoom," | \n");
                    strcat(messagetoSelf, " | <-- You \n");
                    send_message_to_room(messagetoRoom, cli->currentRoomid, cli->uid);
                    send_message_sender(messagetoSelf, cli->connfd);
                }
            } else {
                second = strtok(NULL," ");
                third = strtok(NULL, " ");
                if(!strcmp(first, "-whoami")) {
                    send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                    send_message_sender("Your Nickname : ", cli->connfd);
                    send_message_sender(cli->name, cli->connfd);
                    send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                } else if(!strcmp(first, "-nick")){
                    if(strlen(second) > 1){
                        strcpy(cli->name, second);
                        send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                        send_message_sender("New nickname assigned for you -> ", cli->connfd);
                        send_message_sender(second, cli->connfd);
                        send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                    }
                } else if(!strcmp(first, "-help")){
                    send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                    send_message_sender(helpString, cli->connfd);
                    send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                } else if(!strcmp(first, "-create")) {
                    if(cli->currentRoomid == -1){
                        if(check_room_name(second)){
                            pthread_mutex_lock(&room_mutex);

                            room_t *rm = (room_t *)malloc(sizeof(room_t));
                            strcpy(rm->name, second);
                            rm -> isPrivate = 0;
                            strcpy(rm -> pass, " "); 
                            rm->usercount = 0;
                            rm->actualusers = 1;
                            rm -> roomRemoved = 0;
                            rm -> userids[rm->usercount++] = cli->uid;
                            rm -> roomid = room_count; 
                        
                            pthread_mutex_unlock(&room_mutex);
                            add_to_rooms(rm);
                        } else {
                            send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                            send_message_sender("Room with that name already exist.", cli->connfd);
                            send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                        }
                    } else {
                        send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                        send_message_sender("You are already in a room.", cli->connfd);
                        send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                    }
                } else if(!strcmp(first, "-pcreate")) {
                    if(cli->currentRoomid == -1){
                        if(check_room_name(second)){
                            pthread_mutex_lock(&room_mutex);

                            room_t *rm = (room_t *)malloc(sizeof(room_t));
                            strcpy(rm->name, second);
                            rm -> isPrivate = 1;
                            strcpy(rm -> pass, third); 
                            rm->usercount = 0;
                            rm->actualusers = 1;
                            rm->roomRemoved = 0;
                            rm -> userids[rm->usercount++] = cli->uid;
                            rm -> roomid = room_count; 
                        
                            pthread_mutex_unlock(&room_mutex);
                            add_to_rooms(rm);
                        } else{
                            send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                            send_message_sender("Room with that name already exist.", cli->connfd);
                            send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                        }     
                    } else{
                        send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                        send_message_sender("You are already in a room.", cli->connfd);
                        send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                    }
                } else if(!strcmp(first, "-list")){
                    if(cli->currentRoomid == -1){
                            list_rooms(cli);
                    }
                } else if(!strcmp(first, "-enter")){

                    if(cli->currentRoomid == -1){
                        int idToEnter = find_room_id_by_name(second);
                        if(idToEnter == -1 ){
                            send_message_sender("\n ~~~~~~  No room with that name. ~~~~~~ \n", cli->connfd);
                        } else {
                            if(rooms[idToEnter]->isPrivate == 1){
                                if(strlen(third)>1) {
                                    if(!strcmp(rooms[idToEnter]->pass, third)){
                                        send_message_sender("\n ~~~~~~ Password Correct. Entering the Room. ~~~~~~ \n", cli->connfd);
                                        send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ", cli->connfd);
                                        send_message_sender(rooms[idToEnter]->name, cli->connfd);
                                        send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  \n", cli->connfd);

                                        pthread_mutex_lock(&room_mutex);
                                        rooms[idToEnter]->userids[rooms[idToEnter] -> usercount] =  cli->uid;
                                        rooms[idToEnter]->usercount++;
                                        rooms[idToEnter]->actualusers++;
                                        pthread_mutex_unlock(&room_mutex);

                                        pthread_mutex_lock(&clients_mutex);
                                        cli->currentRoomid = idToEnter; 
                                        pthread_mutex_unlock(&clients_mutex);
                                    } else{
                                        send_message_sender("\n ~~~~~~  Wrong Password. ~~~~~~ \n", cli->connfd);
                                    }
                                } else{
                                    send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ", cli->connfd);
                                    send_message_sender(rooms[idToEnter]->name, cli->connfd);
                                    send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  \n", cli->connfd);

                                    pthread_mutex_lock(&room_mutex);
                                    rooms[idToEnter]->userids[rooms[idToEnter] -> usercount] =  cli->uid;
                                    rooms[idToEnter]->usercount++;
                                    rooms[idToEnter]->actualusers++;
                                    pthread_mutex_unlock(&room_mutex);

                                    pthread_mutex_lock(&clients_mutex);
                                    cli->currentRoomid = idToEnter; 
                                    pthread_mutex_unlock(&clients_mutex);
                                }
                            }
                            
                        }
                    }              
                } else if(!strcmp(first, "-quit")){
                    if(cli->currentRoomid != -1){

                        pthread_mutex_lock(&clients_mutex);
                        pthread_mutex_lock(&room_mutex);

                        rooms[cli->currentRoomid]->actualusers--;

                        if(rooms[cli->currentRoomid]->actualusers == 0){
                            rooms[cli->currentRoomid]->roomRemoved = 1;
                        }
                        cli->currentRoomid =-1;  

                        pthread_mutex_unlock(&clients_mutex);
                        pthread_mutex_unlock(&room_mutex);

                        send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                        send_message_sender("You left the room.", cli->connfd);
                        send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                    } else{
                        send_message_sender("  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                        send_message_sender("You are not in any room.", cli->connfd);
                        send_message_sender("\n  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~  ~~~~~~ \n", cli->connfd);
                    }
                } else if(!strcmp(first, "-exit")){
                    if(cli->currentRoomid != -1){
                        pthread_mutex_lock(&clients_mutex);
                        pthread_mutex_lock(&room_mutex);
                        rooms[cli->currentRoomid]->actualusers--;

                        if(rooms[cli->currentRoomid]->actualusers == 0){
                            rooms[cli->currentRoomid]->roomRemoved = 1;
                        }
                        cli->currentRoomid =-1;  
                        pthread_mutex_unlock(&clients_mutex);
                        pthread_mutex_unlock(&room_mutex);
                    }

                    send_message_sender("Exiting \n",cli->connfd);
                    break;
                }       
            }
        }
    }

    return EXIT_SUCCESS;
}