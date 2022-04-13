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
#include <pthread.h>

#define MAXPRIORITY 5
#define MINPRIORITY 1
#define MICROSECONDOS 1000000

//struct para pasar los parametros a la función y no usar variables globales
struct parametros {
    unsigned minBurst;
    unsigned maxBurst;

    unsigned minEspera;
    unsigned maxEspera;
}; typedef struct parametros T_Parametros;

//Variable que se usará para detener la generación de datos.
int flag = 1;


void *server(){

    int listen_fd, comm_fd;

    struct sockaddr_in servaddr;

    char mensaje[1024];
    char respuesta[1024];

    //abrimos el socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    //se llena el socket con ceros 
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET; //conexion con otros navegadores

    servaddr.sin_addr.s_addr = htons(INADDR_ANY); //abierto a cualquier cliente

    servaddr.sin_port = htons(22500);//puerto 22500 especial para los clientes.

    bind(listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)); //atender al servicio

    listen(listen_fd, 10);//socket y lista de espera de clientes


    while (flag){

        comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL); //acepta la llamada

        //Prepara el buffer para recibir la informacion
        bzero(mensaje, 1024);
        recv(comm_fd, mensaje, sizeof(mensaje), 0);
        //printf("\nMensaje: %s",mensaje);


        if ( strstr(mensaje,"STOP") ){
            flag = 0;
            //Responde al cliente que le mando el mensaje
            bzero(respuesta, 1024);
            strcpy(respuesta, "\n\tCliente automatico detenido.\n");

            send(comm_fd, respuesta, strlen(respuesta), 0);
        }
        bzero(mensaje, 1024);
        bzero(respuesta, 1024);

        //Cierra conexion
        close(comm_fd);
    } 

    return NULL;        

}



void *enviarMensaje(void *arg){
    int sockfd;
    struct sockaddr_in servaddr;

    char *buffer;
    buffer = (char *)arg;

    char buffer_Aux[1024];


    //Crea el socket para llamar al servidor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(22000);

    //La IP cambia para cada maquina donde lo corra
    inet_pton(AF_INET, "10.0.2.15", &(servaddr.sin_addr));

    //Espera 2 segundos antes de enviar el mensaje
    usleep (2*MICROSECONDOS);


    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));//llama

    //printf("Connected to server...\n");

    //bzero(buffer, 1024);
    //strcpy(buffer, "HELLO, PROCESS 01\n");

    printf("Client: %s \n\n", buffer);
    send(sockfd, buffer, strlen(buffer), 0);

    

    bzero(buffer_Aux, 1024);
    recv(sockfd, buffer_Aux, sizeof(buffer_Aux),0);

    printf("Server: %s \n", buffer_Aux);

    close(sockfd);
    //bzero(buffer, 1024);
    //printf("\tEnvió el mensaje\n\t............\n");

    return;

}

void *generarDatos(void *arg){
    //casteo el parametro a la estructura.
    T_Parametros *prmts;
    prmts = (T_Parametros *)arg;

    unsigned burst;
    unsigned priority;

    unsigned espera;
    char mensaje[2];
    pthread_t hilo_proceso2;   

    //Usar un flag para detenerlo.
    while(flag){//De momento usa esa condición para hacer las pruebas

        //ESPERA ENTRE minEspera A maxEspera
        espera = random() % ((prmts->maxEspera) - (prmts->minEspera)+1) + (prmts->minEspera);
        //Usa la variable para pausar el hilo.
        usleep (espera*MICROSECONDOS);
        
        burst = random() % ((prmts->maxBurst) - (prmts->minBurst)+1) + (prmts->minBurst);
        priority = random() % (MAXPRIORITY-MINPRIORITY+1) + MINPRIORITY; 

        //Los convierte en un string
        bzero(mensaje, 2);
        fflush(NULL);
        
        mensaje[0] = burst+'0';
        mensaje[1] = priority+'0';
        

        //ENVÍA LA INFORMACIÓN        
        pthread_create (&hilo_proceso2, NULL, enviarMensaje, (void *) &mensaje);

        /*//PRUEBA
        printf("Prioridad: %u. ", priority);
        printf("Burst: %u. ", burst);
        printf("Mensaje: %s. \n", mensaje);
        printf("-------------------------\n");
        //*/

            
    }
    pthread_join (hilo_proceso2, NULL );
    return;
}


int main(int argc, char *argv[]) {
    if (argc<5){
         printf("Debe ingresar los valores para el rango de burst y el tiempo de espera.\n");
         printf("Los primeros 2 parámetros corresponde al rango de burst y deben de ir de 1 a 9.\n");
         printf("Los últimos 2 parámetros corresponde al rango de espera.\n");
         return 0;
    }

    srandom(time(NULL));//Establecer una raíz

    pthread_t hilo1, hilo2;

    T_Parametros prmts;
    
    prmts.minBurst = atoi( argv[1] ); prmts.maxBurst = atoi( argv[2] );
    prmts.minEspera = atoi( argv[3] ); prmts.maxEspera = atoi( argv[4] );

    pthread_create (&hilo2, NULL, server, NULL);
    pthread_create (&hilo1, NULL, generarDatos, (void *) &prmts);
    
    pthread_join (hilo1, NULL);
    pthread_join (hilo2, NULL);

    printf("\n\tCliente automatico detenido.\n");

    return 0;

}


