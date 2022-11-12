/*Este módulo encarrega-se de receber um URL e dividir todas as suas componentes*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getDetails(int argc, char *argv[], char* path, char* host) {

    char user[100] = "", passwd[100] = "";
    

    sscanf(argv[1], "ftp://%[^/]/%s", host, path);
    //fazer em dois passos e depois verificar
    sscanf(host, "%[^:]:%[^@]@%[^/]", user, passwd, host);
    
    if(strlen(passwd)==0){
        memset(user,0,sizeof(user));
        user[0]='\0';
    }
    printf("\nuser:%s\npasswd:%s\nhost:%s\npath:%s\n", user, passwd, host, path);
    
    return 0;
}
