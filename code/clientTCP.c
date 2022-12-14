/**      (C)2000-2021 FEUP
 *       tidy up some includes and parameters
 * */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>


//Esta função lê o socket e retorna o status code da ultima linha da mensagem (funciona mesmo se so tiver uma linha)
int getLastLineStatusCode(char *buf){
    int a;
    char *pt;
    pt = strtok (buf,"\n");
    while (pt != NULL) {
        a = 0;
        a = atoi(pt);
        pt = strtok (NULL, "\n");
    }
    return a;
}

int getFilename(char *buf, char* filename){
    char *pt;
    pt = strtok (buf,"/");

    while (pt != NULL) {
        strcpy(filename, pt);
        pt = strtok (NULL, "/");
    }

    return 0;
}

int getPortNumber(char* buf){
    int numb[5] = {0};
    int i = 0;
    char *pt;
    pt = strtok (buf,",");
    pt = strtok (NULL,",");
    while (pt != NULL) {
        int a = atoi(pt);
        numb[i] = a;
        pt = strtok (NULL, ",");
        i++;
    }
    return (numb[3]*256 + numb[4]);
}

int createSocket(char* SERVER_ADDR, int SERVER_PORT){
    int sockfd;
    struct sockaddr_in server_addr;
    printf("\n////////// Creating new connection... //////////\n");

    /* criar a outra socket */
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);    /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(SERVER_PORT);        /*server TCP port must be network byte ordered */

    /*open a TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0) {
        perror("socket()");
        return -1;
    }
    /*connect to the server*/
    /* if (connect(sockfd,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect()");
        return -1;
    } */

    connect(sockfd,(struct sockaddr *) &server_addr, sizeof(server_addr));

    return sockfd;
}

int createConnection(char* SERVER_ADDR, int SERVER_PORT, char* user, char* passwd, char* path) {

    //1º abrir a ligaçao com o IP e a PORT
    //2º fazer login (anonymous ou com os dados fornecidos)
    //3º entrar no passive mode
    //4º receber info e calcular a PORT de leitura
    //5º ler de PATH o ficheiro pretendido

    FILE* fileptr;

    int STOP = 0, visited = 0, sizeUser = strlen(user), sizePass = strlen(passwd), port = 0, download = 0, sizePath = strlen(path);

    /* criaçao da string "user anonymous\r\n" "pass qualquer-password\r\n" a funcionar como desejado*/
    char userLogin[sizeUser+7], passwdLogin[sizePass+7], retrvPath[sizePath+7], filename[strlen(path)];
    userLogin[0] = '\0';
    passwdLogin[0] = '\0';
    retrvPath[0] = '\0';

    strcat(userLogin, "user ");
    strcat(passwdLogin, "pass ");
    strcat(retrvPath, "retr ");

    strcat(userLogin, user);
    strcat(passwdLogin, passwd);
    strcat(retrvPath, path);

    strcat(userLogin, "\r\n");
    strcat(passwdLogin, "\r\n");
    strcat(retrvPath, "\r\n");

    getFilename(path, filename);

    char buf[500] = {0}, buf2[500]={0};

    size_t bytes, bytes2;

    int sockfd = createSocket(SERVER_ADDR, SERVER_PORT), sockfd2 = 0;
    if(sockfd == -1) return -1;

    //TODO criar state machine
    while (!STOP)
    {  
        
        memset(buf, 0, 500);
        bytes = read(sockfd, buf, 500);
        
        if(download){
            memset(buf2, 0, 500);
            bytes2 = read(sockfd2, buf2, 500);
            if(bytes2 != -1 && bytes2 != 0) {
                printf("\nbuf2:");
                for(int i=0; i<bytes2; i++){
                    printf("%c", buf2[i]);
                    fputc(buf2[i], fileptr);
                }
            }
        }

        if(bytes == -1 || bytes == 0) {/* printf("\ni got nothing -- buf\n"); */ continue;}
        printf("\n%s\n", buf);
        int sc = getLastLineStatusCode(buf);

        switch(sc){
            case 220: 
                if(visited) break;
                visited = 1;
                printf("\n////////// Sending user... //////////\n");
                write(sockfd, userLogin, strlen(userLogin));
                break;
            case 331:
                printf("\n////////// Sending password... //////////\n");
                write(sockfd, passwdLogin, strlen(passwdLogin));
                break;
            case 230:
                printf("\n////////// Entering passive mode... //////////\n");
                write(sockfd, "pasv\r\n", 6);
                break;
            case 227:
                printf("\n////////// Calculating port number... //////////\n");
                port = getPortNumber(buf);
                printf("\nPort Calculated: %d\n", port);

                if ((sockfd2 = createSocket(SERVER_ADDR, port)) == -1) return -1;

                printf("\n////////// Retrieving file... //////////\n");
                write(sockfd, retrvPath, strlen(retrvPath));


                break;

            case 150: 
                printf("\n////////// Starting File Download... //////////\n");
                fileptr = fopen(filename, "w");
                printf("\n--- Created file with name '%s' ---\n", filename);
                download = 1;
                break;

            case 226: 
                while(1){
                    memset(buf2, 0, 500);
                    bytes2 = read(sockfd2, buf2, 500);
                    if(bytes2 != -1 && bytes2 != 0) {
                        printf("\nbuf2:");
                        for(int i=0; i<bytes2; i++){
                            printf("%c", buf2[i]);
                            fputc(buf2[i], fileptr);
                        }
                        printf("\n");
                    }
                    else{break;}
                }
                printf("\n////////// Ending File Download... //////////\n");
                download = 0;
                STOP = 1;
                printf("\n////////// Closing connection... Bye Bye //////////\n");
                write(sockfd, "quit\r\n", 6);
                break;

            default:
                printf("\n////////// Status code %d not recognized //////////\n", sc);
                return -1;
                break;
        }
    }

    fclose(fileptr);
    
    if (close(sockfd2)<0) {
        perror("close()");
        return -1;
    }

    if (close(sockfd)<0) {
        perror("close()");
        return -1;
    }
    return 0;
}



