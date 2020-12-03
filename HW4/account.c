#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "account.h"

extern Account *head;

int getInput(char buffer[MAXCHAR], char *result) {
    if (buffer[0] != '\n') {
        strtok(buffer, "\n");
        strcpy(result, buffer);
    } else {
        return FALSE;
    }
    return TRUE;
}

Account *createAccount(char username[MAXCHAR], char password[MAXCHAR], int status) {
    Account *temp = (Account *) malloc(sizeof(Account));
    strcpy(temp->username, username);
    strcpy(temp->password, password);
    temp->status = status;
    temp->passIncorrect = 0;
    temp->isAlive = OFFLINE;
    temp->next = NULL;
    return temp;
}

void addAccount(Account **head, Account *node) {
    if( *head == NULL) {
        *head = node;
    } else {
        Account *tmp = *head;
        while (tmp->next != NULL) tmp = tmp->next;
        tmp->next = node;
    }
}

int hasAccount(char username[MAXCHAR]) {
    Account *tmp = head;
    while (tmp != NULL) {
        if (strcmp(tmp->username, username) == 0) {
            return TRUE;
        } 
        tmp = tmp->next;
    }
    return FALSE;
}

int checkPass(Account *account, char password[MAXCHAR]) {
    return strcmp(account->password, password) == 0 ? TRUE : FALSE;
}

Account *getAccount(char username[MAXCHAR]) {
    Account *tmp = head;
    while (tmp != NULL) {
        if (strcmp(tmp->username, username) == 0) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

int signIn(char username[MAXCHAR], char password[MAXCHAR]) {
    
    Account *tmp = getAccount(username);
    if(tmp->status == IDLE) {
        return LOGIN_IDLE;
    } else if (tmp->status == BLOCK) {
        return LOGIN_IDLE;
    }
    
    if(checkPass(tmp, password) == TRUE) {
        tmp->isAlive = ONLINE;
        tmp->passIncorrect = 0;
        printf("Hello %s\n", tmp->username);
        return LOGIN_SUCCESS;
    } else {
        tmp->passIncorrect++;
        if (tmp->passIncorrect >= LIMIT_INCORRECT) {
            tmp->status = BLOCK;
            printf("Password is incorrect. Account is blocked\n");
            return LOGIN_BLOCK;
        } else {
            printf("Password is incorrect\n");
            return LOGIN_FAILURE;
        }
    }
    
}

int changePassword(char username[MAXCHAR], char newpass[MAXCHAR]) {
    Account *tmp = getAccount(username);
    if(checkString(newpass) == FALSE) {
        return FALSE;
    }
    if(tmp->status != BLOCK) {
        strcpy(tmp->password, newpass);
    }
    return TRUE;
}

int checkString(char *str){
    while (*str) {
        if(!isalnum(*str)) return FALSE;
        if (*str == ' ') return FALSE;
        str++;
    }
    return TRUE;
}

void encodePass(char str[MAXCHAR], char *result) {
    char number[MAXCHAR], alpha[MAXCHAR];
    int x = 0, y = 0;
    for(int i = 0; i < strlen(str); i++) {
        if(isdigit(str[i])) {
            number[x++] = str[i];
        } else if (isalpha(str[i])){
            alpha[y++] = str[i];
        }
    }
    number[x] = '\0';
    alpha[y] = '\0';
    strcpy(result, number);
    strcat(result, alpha);
}
