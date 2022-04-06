#include <stdio.h>  
#include <stdlib.h>
#include <string.h>


#define MAXPRIORITY 5
#define MINPRIORITY 1

#define MAXESPERA 8
#define MINESPERA 3

#define MICROSECONDOS 1000000

void *leerDatos(void *arg){

    char *nombre;
    nombre = (char *)arg;

    FILE *archivo;

    unsigned burst;
    unsigned priority;

    char *posiblePuntero;
    int indice0;
    int indice1;

    archivo = fopen(nombre,"r");
    
    if(!archivo){
        printf(stderr, "No se puede abrir el archivo.\n");
        return 0;
    }


    char buffer[1000];//Donde almacena la linea
    char value [1000];//Auxiliar para obtener los valores
    
    while(fgets(buffer, 999, archivo)){


        //Obtiene el puntero de la dirección a ese caracter 
        posiblePuntero = strstr(buffer, ",");
        
        if(!posiblePuntero) {
            printf(stderr, "No coincide el formato del contenido del archivo.\n");
            return 0
        }

        //ESPERAR DE MINESPERA A MAXESPERA ANTES DE OBTENER LOS DATOS
        espera = random() % (MAXESPERA-MINESPERA+1) + MINESPERA;
        //Usa la variable para pausar el hilo.
        usleep (espera*MICROSECONDOS);

        //Obtiene el indice obtener el primer valor
        indice0 = posiblePuntero - buffer;

        //Obtiene el burst
        strncpy(value, buffer + 0, indice0);
        burst = atoi(value);

        
        //Obtiene el puntero de la dirección a ese caracter 
        posiblePuntero = strstr(buffer, "\n");
        
        if(!posiblePuntero) {
            printf(stderr, "No coincide el formato del contenido del archivo.\n");
            return 0
        }

        //Obtiene el indice obtener el segundo valor
        indice1 = posiblePuntero - buffer;
        
        //Obtiene la prioridad
        indice++; 
        strncpy(value, buffer + indice0, indice1-indice0);
        priority = atoi(value);


        //ESPERA DOS SEGUNDOS Y ENVÍA LA INFORMACIÓN
    }
    
    fclose(archivo);
    return 1;

}

int main() {
    char *nombre = "archivo_prueba.txt";

    leerDatos(nombre);
    return 1;

}