#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

void main(int argc, char *argv[]){
    char cadena[100];
    int sockfd;
    struct sockaddr_in servaddr;
    FILE *myf = fopen("Historial_cliente.txt", "a");
    time_t t; 
    struct tm *tm;
    char hora[100];
    char *tmp;
    char sendline[100] = "Proceso xD";
    
    //Crea el socket para llamar al servidor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(22000);
    //Creo que esta IP cambia para cada maquina donde lo corra, revisar
    inet_pton(AF_INET, "192.168.0.10", &(servaddr.sin_addr));
    //==============================
    
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));//llama
    
    printf("\n\n\t\t-----------Se inicio el servidor-----------\n\n");
    fputs("\n\n\t\t-----------Se inicio el servidor-----------\n\n", myf);

    while(!strstr(cadena, "adios") && !strstr(sendline,"adios")){
        
        bzero(cadena, 100);
        //arma el mensaje
        t =time(NULL);
        tm = localtime(&t);
        strftime(hora, 100, "\n Cliente (%H:%M)->", tm );
        printf("%s",hora);
        gets(sendline); //lee de consola
        tmp = strcat(hora, sendline);//manda el mensaje al dar enter
        //arma el mensaje
        fputs(tmp, myf); //archivo
        write(sockfd, sendline, strlen(sendline)+1);
        if(!strstr(cadena, "adios")){
            strftime(hora, 100, "\n Servidor (%H:%M)->", tm);
            read(sockfd, cadena, 100);
            tmp = strcat(hora, cadena);
            printf("%s", tmp);
            fputs(tmp, myf);
        }
        
    }
    printf("\n\n Conversacion finalizada \n");
    printf("\n Se genero el archivo Historial_cliente.txt");
    fclose(myf);
}