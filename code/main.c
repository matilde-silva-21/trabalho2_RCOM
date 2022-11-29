#include <stdio.h>
#include <stdlib.h>
#include "clientTCP.c"
#include "download.c"
#include "getip.c"
/* 
ftp://ftp.up.pt/pub/kodi/timestamp.txt
    pode ser user anon + pass qualquer
*/

/*
ftp://netlab1.fe.up.pt
    user rcom
    pass rcom
*/

int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <URL to deconstruct>\nURL should be as follows: ftp://[<user>:<password>@]<host>/<url-path>\n", argv[0]);
        exit(-1);
    }

    char host[strlen(argv[1])], path[strlen(argv[1])], IPaddress[15]="", user[strlen(argv[1])], password[strlen(argv[1])];

    if(getDetails(argc,argv,path, host, user, password)!=0){
        printf("Error occurred in fuction getDetails (main.c -- line 26)\n");
        exit(-1);
    }
    printf("\n");
    if(getIP(host, IPaddress)!=0){
        printf("Error occurred in fuction getIP (main.c -- line 31)\n");
        exit(-1);
    }
    printf("\n");
    if(createConnection(IPaddress, 21, user, password)!=0){
        printf("Error occurred in fuction createConnection (main.c -- line 36)\n");
        exit(-1);
    }
    printf("\n");
}