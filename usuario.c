#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>


void enviarMensaje(int puerto, int stopServer){
    /*
    Con el parametro puerto indico cual puerto uso para enviar el mensaje
    Si es para los clientes uso 8888 y si es para el server uso 22000
    El parametro stopServer es para indicar si :
        > 0. Mostrar cola pero no parar el server.
        > 1. Mostrar información y parar el server.
        > 2. Para el server sin mostrar nada.
    */

    int sockfd;
    struct sockaddr_in servaddr;

    char buffer[1024];
    char buffer_Aux[1024];


    //Crea el socket para llamar al servidor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(puerto);

    //La IP cambia para cada maquina donde lo corra
    inet_pton(AF_INET, "10.0.2.15", &(servaddr.sin_addr));

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));//llama


    bzero(buffer, 1024);

    if (puerto==22500 || (puerto==22000 && stopServer==2)){
        strcpy(buffer, "STOP");

    }else if (puerto==22000 && stopServer==0){
        strcpy(buffer, "SHOW");

    }else if (puerto==22000 && stopServer==1){//puerto==22000 && stopServer==0
        strcpy(buffer, "AMBAS");
    }

    //Envia el mensaje
    send(sockfd, buffer, strlen(buffer), 0);

    //Usa otra variable para los mensajes recibidos porque afectaba la lectura de datos
    bzero(buffer_Aux, 1024);
    recv(sockfd, buffer_Aux, sizeof(buffer_Aux),0);

    printf(buffer_Aux);

    close(sockfd);
    //bzero(buffer, 1024);
    //printf("\tEnvió el mensaje\n\t............\n");
    return;

}


int menu(){

    char opcion;

    do{
        printf("\n\t\tMenú.\n");
        printf("Seleccione una opción.\n");
        printf("\t1. Detener y mostrar la información.\n");
        printf("\t2. Mostrar información de la cola Ready.\n");
        printf("\t3. Detener la simulación sin información.\n");
        printf("-----------------------------------------\n");        

        do{

            printf( "\n\tIntroduzca opción (1-3): ");
            fflush( stdin );
            scanf( "%c", &opcion );

        } while ( opcion != '1' && opcion != '2' && opcion != '3');

        switch ( opcion ){
            case '1': 
                printf( "Deteniendo la simulación...\n");
                //Para este caso no es necesario los hilos.
                enviarMensaje(22500,0);//Mensaje al cliente. Port 22500
                enviarMensaje(22000,1);//Mensaje al server. Port 22000
                break;

            case '2':
                enviarMensaje(22000,0);
                break;

            case '3':
                printf( "Deteniendo la simulación...\n");  
                enviarMensaje(22500,0);//Mensaje al cliente. Port 22500
                enviarMensaje(22000,2);//Mensaje al server. Port 22000
                break;
        }

    } while ( opcion == '2');

    return 0;
}


int main() {
    
    menu();

    return 0;

}