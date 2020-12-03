#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include "file.h"
#include "account.h"

extern Account *head;
void showMenu();

int main(int argc, char *argv[]) {

    char filename[MAXCHAR];
    int option = 0;

    if (argc <= 1) {
        printf("No input file.\n");
        return -1;
    }
    
    strcpy(filename, argv[1]);
    if(readfile(filename) == 0) return -1;
    
    do {
        showMenu();
        scanf("%d", &option);
        __fpurge(stdin);
        switch(option) {
            case 1:
                registerAccount();          
                writefile(filename);
                break;
            case 2:
                activeAccount();
                writefile(filename);
                break;
            case 3:
                signIn();
                writefile(filename);
                break;
            case 4:
                search();
                break;
            case 5:
                changePassword();
                writefile(filename);
                break;
            case 6:
                logOut();
                break;
            case 7:
                showDomain();
                break;
            case 8:
                showIp();
                break;
        }
    } while (option < 9 && option > 0);
    return 0;
}


void showMenu() {
    printf("\nUSER MANAGEMENT PROGRAM\n");
    printf("-----------------------------------\n");
    printf("1. Register\n");
    printf("2. Activate\n");
    printf("3. Sign in\n");
    printf("4. Search\n");
    printf("5. Change password\n");
    printf("6. Sign out\n");
    printf("7. Homepage with domain name\n");
    printf("8. Homepage with IP address\n");
    printf("Your choice (1-8, other to quit):");
}

