#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <string.h>
#include "Planner.c"



void main(int argc, char *argv[]){
    if (argc > 1){ //contador de parametros
        //char cadena[100];
        int listen_fd, comm_fd, burst, priority;
        struct sockaddr_in servaddr;
        
        printf("Planner con algoritmo: %s \n ", argv[1]);
        Planner *planner = createPlanner(atoi(argv[1])); // el parametro pasado por consola es el algoritmo que debe usar 
        


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
            //printf("Cliend Connected\n");
            //Prepara el buffer para recibir la informacion
            bzero(buffer, 1024);
            recv(comm_fd, buffer, sizeof(buffer), 0);
            //printf("New Process : %s\n", buffer);
            
            //Toma la informacion del socket
            //printf("Server response: Burst:%c Priority: %c \n",buffer[0], buffer[1]);
            burst = buffer[0] - 48;
            priority = buffer[1] - 48;

            //Manda los datos para hacer procesos, a la cola del planner    
            insertData(burst, priority, planner);
            
            //Responde al cliente que le mando el mensaje
            bzero(buffer, 1024);
            strcpy(buffer, "This is server, process end!!!!");
            send(comm_fd, buffer, strlen(buffer), 0);

            //Cierra conexion
            close(comm_fd);
            
            
        }
    }else{
        printf("Parametro esperdado \n");
    }
}
