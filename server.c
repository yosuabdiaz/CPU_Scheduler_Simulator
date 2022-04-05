#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <string.h>

void main(int argc, char *argv[]){
    if (argc > 1){ //contador de parametros
        //char cadena[100];
        int listen_fd, comm_fd;
        struct sockaddr_in servaddr;
        //FILE *myf = fopen("Historial.txt", "a");
        //time_t t; 
        struct tm *tm;
        char hora[100];
        char *tmp;
        char sendline[100] = "Proceso xD";
        char buffer[1024];
        
        //abrimos el socket
        listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        //se llena el socket con ceros 
        bzero(&servaddr, sizeof(servaddr));

        servaddr.sin_family = AF_INET; //conexion con otros navegadores
        servaddr.sin_addr.s_addr = htons(INADDR_ANY); //abierto a cualquier cliente
        servaddr.sin_port = htons(22000);//puesto por el que se manda la info

        bind(listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)); //atender al servicio
        printf("Listening Client...\n");
        listen(listen_fd, 10);//socket y lista de espera de clientes
        
        while (1){
            
            comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL); //acepta la llamada
            printf("Cliend Connected\n");

            bzero(buffer, 1024);
            recv(comm_fd, buffer, sizeof(buffer), 0);
            printf("New Process : %s\n", buffer);

            bzero(buffer, 1024);
            strcpy(buffer, "This is server, process end!!!!");
            printf("Server response: %s \n",buffer);
            send(comm_fd, buffer, strlen(buffer), 0);

            close(comm_fd);
            printf("Client closed\n");

        }
    }else{
        printf("Parametro esperdado \n");
    }
}
