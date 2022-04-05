#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <string.h>

void main(){

    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];

    for (int i=0; i<4; i++){
        //Crea el socket para llamar al servidor
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        bzero(&servaddr, sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(22000);
        //Creo que esta IP cambia para cada maquina donde lo corra, revisar
        inet_pton(AF_INET, "192.168.0.10", &(servaddr.sin_addr));
    
    
        connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));//llama

        printf("Connected to server...\n");

        bzero(buffer, 1024);
        strcpy(buffer, "HELLO, PROCESS 01\n");

        printf("Client: %s \n", buffer);
        send(sockfd, buffer, strlen(buffer), 0);
    }
    

    bzero(buffer, 1024);
    recv(sockfd, buffer, sizeof(buffer),0);

    printf("Server: %s \n", buffer);

    close(sockfd);
    printf("Server Closed\n");
}
