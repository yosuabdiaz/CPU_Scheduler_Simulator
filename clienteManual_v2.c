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

int flag=1;

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
            strcpy(respuesta, "\n\tCliente manual detenido.\n");

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
    

    while(fgets(linea, 999, archivo) && flag==1){

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
        pthread_join (hilo_proceso, NULL );//Espera a que se envié el último mensaje


        /*//PRUEBA
        printf("Prioridad: %u. ", priority);
        printf("Burst: %u. ", burst);
        printf("Espera: %u. \n", espera);
        printf("-------------------------\n");
        //*/
        if (flag == 0){
            break;
        }

    }    

    fclose(archivo);
    return 0;

}





int main(int argc, char *argv[]) {
    if (argc<2){
         printf("Debe ingresar el nombre del archivo con la extención\n");
         return 0;
    }
    char *nombre = argv[1];

    pthread_t hilo1, hilo2;
    pthread_create (&hilo2, NULL, server, NULL);
    pthread_create (&hilo1, NULL, leerDatos, (void *)nombre);
    
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    printf("\n\tCliente manual detenido.\n");

    return 0;
}