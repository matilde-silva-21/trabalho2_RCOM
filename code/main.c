#include <stdio.h>
#include <stdlib.h>
#include "clientTCP.c"
#include "download.c"
#include "getip.c"
/* ftp://ftp.up.pt/pub/kodi/timestamp.txt */

int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <URL to deconstruct>\nURL should be as follows: ftp://[<user>:<password>@]<host>/<url-path>\n", argv[0]);
        exit(-1);
    }

    char host[strlen(argv[1])], path[strlen(argv[1])], IPaddress[15]="";


    if(getDetails(argc,argv,path, host)!=0){
        printf("Error occurred in fuction getDetails (main.c -- line 18)\n");
        exit(-1);
    }
    printf("\n");
    if(getIP(host, IPaddress)!=0){
        printf("Error occurred in fuction getIP (main.c -- line 23)\n");
        exit(-1);
    }
    printf("\n");
    if(createConnection(IPaddress, 21)!=0){
        printf("Error occurred in fuction createConnection (main.c -- line 28)\n");
        exit(-1);
    }
    printf("\n");
}