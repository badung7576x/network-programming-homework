#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int host_to_ip(char *host, char* output)
{
    int i;
	struct hostent *he;
    struct in_addr **addr_list;
         
    if ( (he = gethostbyname(host) ) == NULL) return 0;
 
    addr_list = (struct in_addr **) he->h_addr_list;
     
    for(i = 0; addr_list[i] != NULL; i++)  {
        strcpy(output , inet_ntoa(*addr_list[i]) );
        return 1;
    }

	return 0;
}

// getnameinfo()
int ip_to_host(char *ip, char* output) {
    
    struct in_addr addr;
    struct hostent *hp;

    if (!inet_aton(ip, &addr))
        return 0;

    if ((hp = gethostbyaddr((const char *)&addr, sizeof addr, AF_INET)) == NULL)
        return 0;

    strcpy(output, hp->h_name);

    return 1;
}