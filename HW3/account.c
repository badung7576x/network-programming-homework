#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"
#include "validate.h"
#include "convert.h"

extern Account *head;

int getInput(char inputStr[MAXCHAR], char *result) {
    printf("%s: ", inputStr);
    char tmp[MAXCHAR];
    fgets(tmp, MAXCHAR, stdin);
    if (tmp[0] != '\n') {
        strtok(tmp, "\n");
        strcpy(result, tmp);
    } else {
        return FALSE;
    }
    return TRUE;
}

Account *createAccount(char username[MAXCHAR], char password[MAXCHAR], int status, char homepage[MAXCHAR]) {
    Account *temp = (Account *) malloc(sizeof(Account));
    strcpy(temp->username, username);
    strcpy(temp->password, password);
    temp->status = status;
    strcpy(temp->homepage, homepage);
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

void registerAccount() {
    char username[MAXCHAR], password[MAXCHAR], homepage[MAXCHAR];

    if(getInput("Username", username) == FALSE) {
        printf("Username is empty. Can't register\n");
        return;
    }
    if(hasAccount(username) == FALSE) {
        if (getInput("Password", password) == FALSE) {
            printf("Password is empty. Can't register\n");
            return;
        }
        printf("Homepage: ");
        fgets(homepage, MAXCHAR, stdin);
        strtok(homepage, "\n");
        if(has_space(username) == TRUE || has_space(password) == TRUE) {
            printf("Username or password has space. Can't register\n");
            return;
        } 
        if(has_space(homepage) == TRUE || is_ip_address(homepage) == FALSE || is_host_name(homepage) == FALSE) {
            printf("Homepage is wrong!\n");
        }
        Account *tmp = createAccount(username, password, IDLE, homepage);
        addAccount(&head, tmp);
        printf("Successful registration. Activation required.\n");
    } else {
        printf("Account existed\n");
    }
}

void activeAccount() {
    char username[MAXCHAR], password[MAXCHAR], acctiveCode[MAXCHAR];
    
    getInput("Username", username);
    if(hasAccount(username) == TRUE) {
        Account *tmp = getAccount(username);
        getInput("Password", password);
        getInput("Active code", acctiveCode);
        if(checkPass(tmp, password) == TRUE) {
            if (tmp->status == ACTIVE) {
                printf("Account is activated\n");
                return;
            } else if (tmp->status == BLOCK) {
                printf("Account is blocked\n");
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
    getInput("Username", username);
    if(hasAccount(username) == TRUE) {
        Account *tmp = getAccount(username);
        if(tmp->status == IDLE) {
            printf("Account is not activated.\n");
            return;
        } else if (tmp->status == BLOCK) {
            printf("Account is blocked.\n");
            return;
        }
        getInput("Password", password);
        if(checkPass(tmp, password) == TRUE) {
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
    getInput("Username", username);
    if(hasAccount(username) == 1) {
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
    char username[MAXCHAR], password[MAXCHAR], newpass[MAXCHAR];
    
    getInput("Username", username);
    if(hasAccount(username) == TRUE) {
        getInput("Password", password);
        if(getInput("NewPassword", newpass) == FALSE) {
            printf("Password is empty. Can't register\n");
            return;
        }
        Account *tmp = getAccount(username);
        if(checkPass(tmp, password) == TRUE) {
            if (strcmp(password, newpass) != 0) {
                tmp->passIncorrect = 0;
                strcpy(tmp->password, newpass);
                printf("Password is changed\n");
            } else {
                printf("The new password is the same as the old one! Password isn't change\n");
                return;
            } 
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
    
    getInput("Username", username);
    if(hasAccount(username) == TRUE) {
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

void showDomain() {
    char username[MAXCHAR], host[MAXCHAR];
    getInput("Username", username);
    if(hasAccount(username) == TRUE) {
        Account *tmp = getAccount(username);
        if(tmp->status == IDLE) {
            printf("Account is not activated.\n");
            return;
        } else if (tmp->status == BLOCK) {
            printf("Account is blocked.\n");
            return;
        }
        if(tmp->isAlive == ONLINE) {
            if(is_ip_address(tmp->homepage) == TRUE) {
                if(ip_to_host(tmp->homepage, host) == TRUE) {
                    printf("Official name: %s\n", host);
                } else {
                    printf("Not found information\n");
                }
            } else if (is_host_name(tmp->homepage) == TRUE) {
                printf("Official name: %s\n", tmp->homepage);
            } else {
                printf("Not found information\n");
            }
        } else {
            printf("Account is not sign in\n");
        }
    } else {
        printf("Cannot find account\n");
    }
}

void showIp() {
    char username[MAXCHAR], ip[MAXCHAR];
    getInput("Username", username);
    if(hasAccount(username) == TRUE) {
        Account *tmp = getAccount(username);
        if(tmp->status == IDLE) {
            printf("Account is not activated.\n");
            return;
        } else if (tmp->status == BLOCK) {
            printf("Account is blocked.\n");
            return;
        }
        if(tmp->isAlive == ONLINE) {
            if(is_host_name(tmp->homepage) == TRUE) {
                if(host_to_ip(tmp->homepage, ip) == TRUE) {
                    printf("Official IP: %s\n", ip);
                } else {
                    printf("Not found information\n");
                }
            } else if (is_ip_address(tmp->homepage) == TRUE) {
                printf("Official IP: %s\n", tmp->homepage);
            } else {
                printf("Not found information\n");
            }
        } else {
            printf("Account is not sign in\n");
        }
    } else {
        printf("Cannot find account\n");
    }
}
