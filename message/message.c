#include "message.h"

message *string_to_message(char *mess) {
    message *tmp = (message *) malloc(sizeof(message));
    
    int i = 0, start = 0, end = 0, step = 0;
    while(i < strlen(mess)) {
        if(mess[i] == '[') {
            start = i + 1;
        } else if (mess[i] == ']') {
            end = i;
            char subStr[1000];
            int k = 0;
            while(k < end - start) {
                subStr[k] = mess[start + k];
                k++;
            }
            subStr[k] = '\0';
            if(step == 0) { strcpy(tmp->username, subStr); step++;}
            else if(step == 1) { strcpy(tmp->code, subStr); step++;}
            else if(step == 2) { strcpy(tmp->room, subStr); step++;}
            else if(step == 3) { strcpy(tmp->message, subStr); step++;}
        } 
        i++;
        
    }
    return tmp;
}

void print_message(message *messObj) {
    printf("Username: %s, Code: %s, Room: %s, Message: %s \n", messObj->username, messObj->code, messObj->room, messObj->message);
}