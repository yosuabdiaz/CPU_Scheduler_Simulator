#include <stdio.h>
#include "Simple_queue.c"
#include "Queue.c"
#include "JobSchedulerThread.c"
#include "CPUSchedulerThread.c"
#include "Planner.h"

void insertData(int burst, int priority, Planner *p){
    Insert_simple_queue(burst, priority, p->dataQueue);
    printf("Inserted: %i %i \n",burst, priority);
}


