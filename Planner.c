#include <stdio.h>
#include "Simple_queue.c"
#include "Queue.c"
#include "JobSchedulerThread.c"
#include "CPUSchedulerThread.c"
#include <time.h> 


typedef struct Planner {
    JobSchedulerThread *jobSchedulerThread;
    CPUSchedulerThread *cpuSchedulerThread;
    Queue *readyQueue;
    long int time;

} Planner;


//Constructor 
Planner * createPlanner(int tipoAlgoritmo, int quantum){   
    Planner *p = malloc(sizeof(Planner));
    pthread_t hilo;
    p->readyQueue = createQueue();
    p->time = (clock()/CLOCKS_PER_SEC);

    //Crea un mutex para ser compartido entre el Job y el CPU para accesar a la cola del ready
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


    p->jobSchedulerThread = createJobScheduler(p->time,&mutex,p->readyQueue);
    p->cpuSchedulerThread = createCPUScheduler(p->time,tipoAlgoritmo,quantum,&mutex,p->readyQueue);
    

    return p;

}

void insertData(int burst, int priority, Planner *p){
    Insert_simple_queue(burst, priority, p->jobSchedulerThread->dataQueue);
    printf("Inserted: %i %i \n",burst, priority);

}




