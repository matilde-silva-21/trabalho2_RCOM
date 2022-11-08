/*Este módulo encarrega-se de receber um URL e dividir todas as suas componentes*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <URL to deconstruct>\nURL should be as follows: ftp://[<user>:<password>@]<host>/<url-path>\n", argv[0]);
        exit(-1);
    }

    int lastCut = 6;
    char user[100] = "", passwd[100] = "", host[100] = "", readingPath = 0, path[300] = "";
    

    sscanf(argv[1], "ftp://%[^:]:%[^@]@%[^/]/%s", user, passwd, host, path);
    
    printf("\nuser:%s\npasswd:%s\nhost:%s\npath:%s\n", user, passwd, host, path);
    
    return 0;
}
