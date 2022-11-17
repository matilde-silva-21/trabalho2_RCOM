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


int createConnection(char* SERVER_ADDR, int SERVER_PORT) {

    //1º abrir a ligaçao com o IP e a PORT
    //2º fazer login (anonymous ou com os dados fornecidos)
    //3º entrar no passive mode
    //4º receber info e calcular a PORT de leitura
    //5º ler de PATH o ficheiro pretendido

    int sockfd, phase = 0;
    struct sockaddr_in server_addr;
    char buf[100] = "user anonymous\n";
    size_t bytes;

    /*server address handling*/
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);    /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(SERVER_PORT);        /*server TCP port must be network byte ordered */

    /*open a TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return -1;
    }
    /*connect to the server*/
    if (connect(sockfd,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect()");
        return -1;
    }

    bytes = read(sockfd, buf, 100);

    //TODO criar state machine
    
    /* while((bytes = write(sockfd, buf, strlen(buf))) != -1){
        if (bytes > 0)
            printf("Bytes escritos %ld\n", bytes);
        else {
            perror("write()");
            return -1;
        }

        bytes = read(sockfd, buf, 100);

        if(strncmp(buf, "220", 3) == 0){
            printf("\n%s\n", buf);
            memset(buf,0,100);
        }

        if(strncmp(buf, "331", 3) == 0){
            printf("\n%s\n", buf);
            memset(buf,0,100);
            strcat("pass 1234\0",buf);
        }

        else if(strncmp(buf, "230", 3) == 0){
            printf("\n%s\n", buf);
            memset(buf,0,100);
            strcat("pasv\0",buf);
        }

        else if(strncmp(buf, "227", 3) == 0){
            printf("\n%s\n", buf);
        }

        else if(strncmp(buf, "150", 3) == 0){
            
        }

        else if(strncmp(buf, "226", 3) == 0){
            
        }

        else{
            printf("\nStatus code not expected: %c%c%c\n", buf[0], buf[1], buf[2]);
        }

        sleep(7);
        
    }
     */

    if (close(sockfd)<0) {
        perror("close()");
        return -1;
    }
    return 0;
}


