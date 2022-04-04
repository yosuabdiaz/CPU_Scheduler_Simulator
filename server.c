#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

void main(int argc, char *argv[]){
    if (argc > 1){ //contador de parametros
        char cadena[100];
        int listen_fd, comm_fd;
        struct sockaddr_in servaddr;
        FILE *myf = fopen("Historial.txt", "a");
        time_t t; 
        struct tm *tm;
        char hora[100];
        char *tmp;
        char sendline[100] = "Proceso xD";

        //abrimos el socket
        listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        //se llena el socket con ceros 
        bzero(&servaddr, sizeof(servaddr));

        servaddr.sin_family = AF_INET; //conexion con otros navegadores
        servaddr.sin_addr.s_addr = htons(INADDR_ANY); //abierto a cualquier cliente
        servaddr.sin_port = htons(22000);//puesto por el que se manda la info

        bind(listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)); //atender al servicio
        listen(listen_fd, 10);//socket y lista de espera de clientes
        comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL); //acepta la llamada

        printf("\n\n\t\t-----------Se inicio el servidor-----------\n\n");
        fputs("\n\n\t\t-----------Se inicio el servidor-----------\n\n", myf);
        
        char *mode;//captura el parametro para tipo de algoritmo
        mode = argv[1]; // sacarlo de la lista de parametros
        while(!strstr(cadena, "adios")&& !strstr(sendline, "adios")){
            switch (*mode){ // se escoge, no se como sera
                case 'f':
                    printf("fifo");
                    break;
                
                case 's':
                    printf("SJF");
                    break;
                
                case 'h':
                    printf("HPF");
                    break;
                
                case 'r':
                    printf("Round Robin");
                    break;

                default:
                    printf("tipo no valido");
                    break;
            }
            bzero(cadena, 100);//llena de ceros el socket
            t = time(NULL);  //hora
            tm = localtime(&t); //para ver la hora que entro el mensaje
            strftime(hora, 100, "\n Cliente (%H:%M)->", tm); //Arma el mensaje
            
            read(comm_fd, cadena, 100); //leer de la llamada
            tmp=strcat(hora, cadena); // concatena la info
            printf("%s", tmp); //consola
            fputs(tmp, myf);    //escribir archivo
            if(!strstr(cadena, "adios")){
                strftime(hora, 100, "\n Servidor (%H:%M)->", tm);
                printf("%s", hora); //consola
                gets(sendline); //leer de consola
                tmp = strcat(hora, sendline); //manda el mensaje al dar enter
                write(comm_fd, sendline, strlen(sendline)+1);//manda el mensaje
                fputs(tmp,myf);  //escribir archivo
            }
            printf("\n\n\t\t-----------Se termino el servidor-----------\n\n");
            printf("\n\n\t\t-----------Se genero Historial.txt-----------\n\n");
        fclose(myf);
        }
    }else{
        printf("Parametro esperdado \n");
    }
}
