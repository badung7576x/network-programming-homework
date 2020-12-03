#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validate.h"
#include "convert.h"
#define MAXCHAR 100
#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        printf("Missing parameters!\n");
        return -1;
    }
    int parameter1;
    char parameter2[MAXCHAR];
    char ip[MAXCHAR];
    char host[MAXCHAR];

    parameter1 = atoi(argv[1]);
    strcpy(parameter2, argv[2]);

    if (parameter1 == 1) {
        if(is_ip_address(parameter2) == TRUE) {
            if(ip_to_host(parameter2, host) == TRUE) {
                printf("Official name: %s\n", host);
            } else {
                printf("Not found information\n");
            }
        } else {
            printf("Wrong parameter\n");
        }
    } else if (parameter1 == 2)
    {
        if(is_host_name(parameter2) == TRUE) {
            if(host_to_ip(parameter2, ip) == TRUE) {
                printf("Official IP: %s\n", ip);
            } else {
                printf("Not found information\n");
            }
        } else {
            printf("Wrong parameter\n");
        }
    } else {
        printf("Wrong parameter\n");
    }
    
    return 0;

}