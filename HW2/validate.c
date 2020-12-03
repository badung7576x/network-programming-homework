#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validate.h"

int is_number(char *str) {
    while (*str) {
        if(!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

int is_ip_address(char *ip) {
    char strip[20];
    strcpy(strip, ip);
    int num, dots = 0;
    char *ptr;
    if (ip == NULL) return 0;
    ptr = strtok(strip, ".");
    if (ptr == NULL) return 0;
    while (ptr) {
        if (!is_number(ptr)) return 0;
        num = atoi(ptr);
        if (num >= 0 && num <= 255) {
            ptr = strtok(NULL, ".");
            if (ptr != NULL) dots++;
        } else return 2;
    }
    if (dots != 3) return 0;
    return 1;
}

int is_host_name(char *host) {
    if(host == NULL) return 0;
    if(isalpha(*host) == 0) return 0;
    return 1;
}