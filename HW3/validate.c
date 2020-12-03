#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validate.h"


int is_number(char *str) {
    while (*str) {
        if(!isdigit(*str)) return FALSE;
        str++;
    }
    return TRUE;
}

int is_ip_address(char *ip) {
    char strip[20];
    strcpy(strip, ip);
    int num, dots = 0;
    char *ptr;
    if (ip == NULL) return FALSE;
    ptr = strtok(strip, ".");
    if (ptr == NULL) return FALSE;
    while (ptr) {
        if (!is_number(ptr)) return FALSE;
        num = atoi(ptr);
        if (num >= 0 && num <= 255) {
            ptr = strtok(NULL, ".");
            if (ptr != NULL) dots++;
        } else return 2;
    }
    if (dots != 3) return FALSE;
    return TRUE;
}

int is_host_name(char *host) {
    char tmp[MAX_CHAR];
    strcpy(tmp, host);
    if(host == NULL) return FALSE;
    if(isalpha(*host) == FALSE) return FALSE;
    if(has_space(tmp) == TRUE) return FALSE;
    return TRUE;
}

int has_space(char *string) {
    char *tmp = string;
    while (*tmp++)
        if (*tmp == ' ')
            return TRUE;
    return FALSE;
}