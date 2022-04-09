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

    //printf("Server: %s \n", buffer_Aux);

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
    while(priority<5){//De momento usa esa condición para hacer las pruebas

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


int main() {
    srandom(time(NULL));//Establecer una raíz

    pthread_t hilo1;

    T_Parametros prmts;
    
    prmts.minBurst=1; prmts.maxBurst=5;
    prmts.minEspera=3; prmts.maxEspera=8;

    pthread_create (&hilo1, NULL, generarDatos, (void *) &prmts);
    
    pthread_join (hilo1, NULL ) ;

    return 0;

}


