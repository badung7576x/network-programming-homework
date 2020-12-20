#include "common.h"

void strip_newline(char *s){
    while (*s != '\0') {
        if (*s == '\r' || *s == '\n') *s = '\0';
        s++;
    }
}