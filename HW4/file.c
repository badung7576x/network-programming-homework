#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "account.h"

Account *head = NULL;

int readfile(char *filename) {
    FILE *fo;
 
    fo = fopen(filename, "r");
    if (fo == NULL) {
        printf("Could not open file %s\n",filename);
        return 0;
    }
    char username[MAXCHAR], password[MAXCHAR];
    int status;
    while( fscanf(fo, "%s %s %d", username, password, &status) != EOF ) {
        Account *tmp = createAccount(username, password, status);
        addAccount(&head, tmp);
    }
    fclose(fo);
    return 1;
}

void writefile(char *filename) {
    FILE *fo;
 
    fo = fopen(filename, "w");
    if (fo == NULL) {
        printf("Could not open file %s\n",filename);
        return;
    }
    
    Account *tmp = head;
    while (tmp != NULL) {
        fprintf(fo, "%s %s %d\n", tmp->username, tmp->password, tmp->status);
        tmp = tmp->next;
    }
    fclose(fo);
}