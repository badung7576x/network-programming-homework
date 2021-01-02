#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char username[32];
    int uid;
    int rid;
    char code[100];
    char message[1024];
} message;

message *string_to_message(char *mess);
void print_message(message *messObj);