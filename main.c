#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Planner.c"
//#include "Simple_queue.c"
int main(void){
    /* */
    //dequeue(q);
    //printQueue(q); 
    Planner *planner = createPlanner(2);     //Tipo es SJF
    int burst =3;
    int priority = 4;
    /* Prueba para meter datos de procesos al planer
    insertData(burst, priority, planner);
    insertData(burst, priority, planner);
    insertData(burst, priority, planner);
    insertData(burst, priority, planner);
    insertData(burst, priority, planner);
    */
    return 0;
}