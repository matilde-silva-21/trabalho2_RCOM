/*Este módulo encarrega-se de receber um URL e dividir todas as suas componentes*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <URL to deconstruct>\nURL should be as follows: ftp://[<user>:<password>@]<host>/<url-path>\n", argv[0]);
        exit(-1);
    }

    int lastCut = 6;
    char user[100] = "", passwd[100] = "", host[100] = "", readingPath = 0, path[300] = "";
    

    sscanf(argv[1], "ftp://%[^/]/%s", host, path);
    //fazer em dois passos e depois verificar
    sscanf(host, "%[^:]:%[^@]@", user, passwd);
    if(strlen(passwd)==0){
        memset(user,0,strlen(user));
        user[0]='\0';
    }
    printf("\nuser:%s\npasswd:%s\nhost:%s\npath:%s\n", user, passwd, host, path);
    
    return 0;
}
