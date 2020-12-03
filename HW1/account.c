#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"

extern Account *head;

Account *createAccount(char username[MAXCHAR], char password[MAXCHAR], int status) {
    Account *temp = (Account *) malloc(sizeof(Account));
    strcpy(temp->username, username);
    strcpy(temp->password, password);
    temp->status = status;
    temp->passIncorrect = 0;
    temp->activeIncorrect = 0;
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

int checkAccount(char username[MAXCHAR]) {
    Account *tmp = head;
    while (tmp != NULL) {
        if (strcmp(tmp->username, username) == 0) {
            return 1;
        } 
        tmp = tmp->next;
    }
    return 0;
}

int checkPass(Account *account, char password[MAXCHAR]) {
    return strcmp(account->password, password) == 0 ? 1 : 0;
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

void registerAccount() {
    char username[MAXCHAR], password[MAXCHAR];
    printf("Username: ");
    scanf("%s", username);
    if(checkAccount(username) == 0) {
        printf("Password: ");
        scanf("%s", password);
        Account *tmp = createAccount(username, password, IDLE);
        addAccount(&head, tmp);
        printf("Successful registration. Activation required.\n");
    } else {
        printf("Account existed\n");
    }
}

void activeAccount() {
    char username[MAXCHAR];
    char password[MAXCHAR];
    char acctiveCode[MAXCHAR];
    printf("Username: ");
    scanf("%s", username);
    if(checkAccount(username) == 1) {
        Account *tmp = getAccount(username);
        printf("Password: ");
        scanf("%s", password);
        printf("Active code: ");
        scanf("%s", acctiveCode);
        if(checkPass(tmp, password) == 1) {
            if (tmp->status == ACTIVE) {
                printf("Account is activated\n");
                return;
            }
            if(tmp->activeIncorrect < LIMIT_INCORRECT) {
                if (strcmp(acctiveCode, ACTIVE_CODE) == 0) {
                    tmp->status = ACTIVE;
                    printf("Account is activated\n");
                } else {
                    tmp->activeIncorrect++;
                    if (tmp->activeIncorrect == LIMIT_INCORRECT) {
                        tmp->status = BLOCK;
                        printf("Activation code is incorrect.\n");
                        printf("Account is blocked\n");
                    } else {
                        printf("Account is not activated\n");
                    }
                }  
            } else {
                printf("Account is blocked\n");
            }
        } else {
            printf("Password is incorrect\n");
        }
    } else {
        printf("Cannot find account\n");
    }
}

void signIn() {
    char username[MAXCHAR], password[MAXCHAR];
    printf("Username: ");
    scanf("%s", username);
    if(checkAccount(username) == 1) {
        Account *tmp = getAccount(username);
        if(tmp->status == IDLE) {
            printf("Account is not activated.\n");
            return;
        } else if (tmp->status == BLOCK) {
            printf("Account is blocked.\n");
            return;
        }
        printf("Password: ");
        scanf("%s", password);
        if(checkPass(tmp, password) == 1) {
            tmp->isAlive = ONLINE;
            tmp->passIncorrect = 0;
            printf("Hello %s\n", tmp->username);
        } else {
            tmp->passIncorrect++;
            if (tmp->passIncorrect >= LIMIT_INCORRECT) {
                tmp->status = BLOCK;
                printf("Password is incorrect. Account is blocked\n");
            } else {
                printf("Password is incorrect\n");
            }
        }
    } else {
        printf("Cannot find account\n");
    }
}

void search() {
    char username[MAXCHAR];
    printf("Username: ");
    scanf("%s", username);
    if(checkAccount(username) == 1) {
        Account *tmp = getAccount(username);
        if(tmp->status == IDLE) {
            printf("Account is not activated.\n");
            return;
        } else if (tmp->status == BLOCK) {
            printf("Account is blocked.\n");
            return;
        } 
        if(tmp->isAlive == ONLINE) {
            printf("Account is active.\n");
        } else {
            printf("Account is not sign in\n");
        }
    } else {
        printf("Cannot find account\n");
    }
}

void changePassword() {
    char username[MAXCHAR];
    char password[MAXCHAR];
    char newpass[MAXCHAR];
    printf("Username: ");
    scanf("%s", username);

    if(checkAccount(username) == 1) {
        printf("Password: ");
        scanf("%s", password);
        printf("NewPassword: ");
        scanf("%s", newpass);
        Account *tmp = getAccount(username);
        if(checkPass(tmp, password) == 1) {
            tmp->passIncorrect = 0;
            strcpy(tmp->password, newpass);
            printf("Password is changed\n");
        } else {
            tmp->passIncorrect++;
            if (tmp->passIncorrect >= LIMIT_INCORRECT) {
                tmp->status = BLOCK;
                printf("Password is incorrect. Account is blocked\n");
            } else {
                printf("Current password is incorrect. Please try again\n");
            }
        }            
    } else {
        printf("Cannot find account\n");
    }
}

void logOut(){
    char username[MAXCHAR];
    printf("Username: ");
    scanf("%s", username);
    if(checkAccount(username) == 1) {
        Account *tmp = getAccount(username);
        if(tmp->isAlive == ONLINE) {
            tmp->isAlive = OFFLINE;
            printf("Goodbye %s. \n", tmp->username);
        } else {
            printf("Account is not sign in\n");
        }
    } else {
        printf("Cannot find account\n");
    }
}


