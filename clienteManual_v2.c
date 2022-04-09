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


#define MAXPRIORITY 5
#define MINPRIORITY 1

#define MAXESPERA 8
#define MINESPERA 3

#define MICROSECONDOS 1000000

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

    
    //Usa otra variable para los mensajes recibidos porque afectaba la lectura de datos
    bzero(buffer_Aux, 1024);
    recv(sockfd, buffer_Aux, sizeof(buffer_Aux),0);

    //printf("Server: %s \n", buffer_Aux);

    close(sockfd);
    //bzero(buffer, 1024);
    //printf("\tEnvió el mensaje\n\t............\n");

    return;

}


void *leerDatos(void *arg){

    char *nombre;
    nombre = (char *)arg;

    FILE *archivo;

    unsigned burst;
    unsigned priority;

    unsigned espera;

    char mensaje[2];

    char *posiblePuntero;
    int indice0;
    int indice1;

    archivo = fopen(nombre,"r");
    
    if(!archivo){
        printf(stderr, "No se puede abrir el archivo.\n");
        return 0;
    }

    printf("\tLeyendo el archivo.\n\t-------------------\n\n");

    char linea[1000];//Donde almacena la linea
    char value [1000];//Auxiliar para obtener los valores
    pthread_t hilo_proceso;
    
    while(fgets(linea, 999, archivo)){


        //Obtiene el puntero de la dirección a ese caracter 
        posiblePuntero = strstr(linea, ",");
        
        if(!posiblePuntero) {
            printf(stderr, "No coincide el formato del contenido del archivo.\n");
            return 0;
        }

        //ESPERAR DE MINESPERA A MAXESPERA ANTES DE OBTENER LOS DATOS
        espera = random() % (MAXESPERA-MINESPERA+1) + MINESPERA;
        //Usa la variable para pausar el hilo.
        usleep (espera*MICROSECONDOS);

        //Obtiene el indice obtener el primer valor
        indice0 = posiblePuntero - linea;

        //Obtiene el burst
        strncpy(value, linea + 0, indice0);
        burst = atoi(value);

        
        //Obtiene el puntero de la dirección a ese caracter 
        posiblePuntero = strstr(linea, "\n");
        
        if(!posiblePuntero) {
            printf("\n\nERROR: No coincide el formato del contenido del archivo: %s\n\n",linea);
            return 0;
        }

        //Obtiene el indice obtener el segundo valor
        indice1 = posiblePuntero - linea;
        
        //Obtiene la prioridad
        indice0++; 
        strncpy(value, linea + indice0, indice1-indice0);
        priority = atoi(value);

        bzero(linea, 1000);
        bzero(value, 1000);
        fflush(NULL);
        //Prepara el mensaje
        mensaje[0] = burst+'0';
        mensaje[1] = priority+'0';


        //ENVÍA LA INFORMACIÓN       
        pthread_create (&hilo_proceso, NULL, enviarMensaje, (void *) &mensaje);

        /*//PRUEBA
        printf("Prioridad: %u. ", priority);
        printf("Burst: %u. ", burst);
        printf("Espera: %u. \n", espera);
        printf("-------------------------\n");
        //*/
    }    
    fclose(archivo);
    
    pthread_join (hilo_proceso, NULL );//Espera a que se envié el último mensaje
    return 0;
}


int main() {
    char *nombre = "archivo.txt";
    
    pthread_t hilo1;
    pthread_create (&hilo1, NULL, leerDatos, (void *)nombre);
    
    pthread_join(hilo1, NULL);

    return 0;

}