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

int flag = 1;

void *generarDatos(void *arg){
    //casteo el parametro a la estructura.
    T_Parametros *prmts;
    prmts = (T_Parametros *)arg;

    unsigned burst;
    unsigned priority;

    unsigned espera;
    char *mensaje[2];    

    //Usar un flag para detenerlo.
    while(priority<5){//De momento usa esa condición para hacer las pruebas

        //ESPERA ENTRE minEspera A maxEspera
        espera = random() % ((prmts->maxEspera) - (prmts->minEspera)+1) + (prmts->minEspera);
        //Usa la variable para pausar el hilo.
        usleep (espera*MICROSECONDOS);
        
        burst = random() % ((prmts->maxBurst) - (prmts->minBurst)+1) + (prmts->minBurst);
        priority = random() % (MAXPRIORITY-MINPRIORITY+1) + MINPRIORITY; 

        //Los convierte en un string
        mensaje[0] = burst+'0';
        mensaje[1] = priority+'0';

        //ENVÍA LA INFORMACIÓN
        
        //pthread_t hilo_proceso;
        //pthread_create (&hilo_proceso, NULL, enviarmensaje, (void *) &mensaje);

        /*//PRUEBA
        printf("Prioridad: %lu. \n", priority);
        printf("Burst: %lu. \n", burst);
        printf("-------------------------\n");
        //*/     
    }
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


