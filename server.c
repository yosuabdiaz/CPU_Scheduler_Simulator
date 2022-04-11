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
#include <pthread.h>

//Bandera para detener las ejecuciones y ejecutar las funciones que solicite el user
int flag = 1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
Simple_queue *dataQueue;
int algoritmo = 0;
int quamtum = 0;
int semaforo = 0;
int burst = 0;
int priority = 0;

void *serverSocket(void *unused){
 //char cadena[100];
    int listen_fd, comm_fd, burst, priority;
    struct sockaddr_in servaddr;
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
    while (flag){
        comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL); //acepta la llamada
        printf("Client Connected\n");
        //Prepara el buffer para recibir la informacion
        bzero(buffer, 1024);
        recv(comm_fd, buffer, sizeof(buffer), 0);
        
        if ( strstr(buffer,"SHOW") ){
            //Si el mensaje contiene SHOW significa que lo envió el usuario para solo mostrar el ready.
            //El flag en 2 para indicar que solo hay que mostrar la cola.
            //PARA ESTO SE PONE EL FLAG EN 2.
            flag = 2;

            //Responde al cliente usuario
            bzero(buffer, 1024);
            strcpy(buffer, "\n\tMostrando Cola del Ready.\n");

            send(comm_fd, buffer, strlen(buffer), 0);
            
        }else if ( strstr(buffer,"STOP") ){
            //Si el mensaje contiene STOP significa que lo envió el usuario para detener todo sin mostrar nada.
            //El flag en 3 para indicar que pare la ejecución del server.
            flag = 3;
            //Responde al cliente usuario
            bzero(buffer, 1024);
            strcpy(buffer, "\n\tServer Detenido.\n");

            send(comm_fd, buffer, strlen(buffer), 0);

        }else if ( strstr(buffer,"AMBAS") ){
            //Si el mensaje contiene AMBAS significa que lo envió el usuario para mostrar y detener todo.
            //El flag en 4 para indicar que hay que mostrar la info y detener todo.
            flag = 4;
            //Responde al cliente usuario
            bzero(buffer, 1024);
            strcpy(buffer, "\n\tMostrando la información y deteniendo el server.\n");

            send(comm_fd, buffer, strlen(buffer), 0);

        }else{
            printf("New Process : %s\n", buffer);
            //Toma la informacion del socket
            //printf("Server response: Burst:%c Priority: %c \n",buffer[0], buffer[1]);
            burst = buffer[0] - 48;
            priority = buffer[1] - 48;
            Insert_simple_queue(burst, priority, dataQueue);
            //Responde al cliente que le mando el mensaje
            bzero(buffer, 1024);
            strcpy(buffer, "This is server, process end!!!!");
            send(comm_fd, buffer, strlen(buffer), 0);
            semaforo = 1;
        }
        
        //Cierra conexion
        close(comm_fd);
    } 
    return NULL;      
}
void *CreatePlanner(char *argv[]){
    Planner *planner = createPlanner(algoritmo, quamtum); // el parametro pasado por consola es el algoritmo que debe usar 
    while (flag)
    {
        if (semaforo == 1){
            insertData(burst ,priority,planner); //Burst,priority
            semaforo = 0;
        }
        
        //Funciones que ejecuta el usuario.
        if(flag==2){ //Mostrar cola pero no parar el server.
            //EJECUTA LA FUNCION QUE MUESTRE LA COLA.
            mostrarColaReady(planner);
            //Regresa el flag a la "normalidad"
            flag=1;

        }else if (flag==3){ //Para el server sin mostrar nada.
            //PARA LA EJECUCIÓN
            flag=0;

        }else if (flag==4){ //Mostrar cola y parar el server.
            //MUESTRA LAS TABLAS Y DEMÁS DATOS.
            //AQUÍ VA LAS FUNCIONES...
            //PARA LA EJECUCIÓN
            flag=0;
        }
    }
    return NULL;
}
void main(int argc, char *argv[]){
    if (argc > 1){ //contador de parametros
        algoritmo = atoi(argv[1]);
        quamtum = atoi(argv[2]);
        pthread_t thread1, thread2;
        pthread_create(&thread1, NULL, serverSocket, NULL);
        dataQueue = createSimpleQueue();
        printf("Planner con algoritmo: %s \n ", argv[1]);
        pthread_create(&thread2, NULL, CreatePlanner, &argv);
        pthread_join ( thread1 , NULL ) ;
        pthread_join ( thread2 , NULL ) ;
        //Si llega hasta acá el server se detuvo.
        printf("\n\tServer Detenido.\n");

    }else{
        printf("Parametro esperdado \n");
    }
}



