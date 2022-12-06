/*Este módulo encarrega-se de receber um URL e dividir todas as suas componentes*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getDetails(int argc, char *argv[], char* path, char* host, char* user, char* passwd, char* dummy) {

    memset(passwd, 0, strlen(passwd));
    memset(user, 0, strlen(passwd));

    sscanf(argv[1], "ftp://%[^/]/%s", host, path);
    //fazer em dois passos e depois verificar
    sscanf(host, "%[^:]:%[^@]@%[^/]", user, passwd, host);

    if(strlen(passwd)==0){
        strcpy(user, "anonymous");
        strcpy(passwd, "qualquer-password");
    }
    printf("\nuser:%s\npasswd:%s\nhost:%s\npath:%s\n", user, passwd, host, path);
    
    return 0;
}
