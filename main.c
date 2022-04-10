#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Planner.c"
#include <time.h> 


int main(void){
    
    //EJEMPLO DE TIMER 
    /*
    long int start;
    start = clock();
    int patata = 0;
    for(patata=0; patata<10000000; patata++){printf("Se han encontrado %d patatas\n", patata);}
    //start = time(NULL) - start;       .3f   (double)
    printf("contar 100000 patatas ha tardado en C: %ld segundos \n",  (clock()-start)/100000);
    printf("%ld", time(NULL));
    */

    Planner *planner = createPlanner(4,4);     //Tipo es RR con Q=4
    //Prueba para meter datos de procesos al planer



    insertData(3, 2, planner); //Burst,priority
    usleep(3*1000000); 
    insertData(5, 1, planner);
  
   
    while(1){}
    return 0;
}