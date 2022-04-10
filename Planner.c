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
    p->readyQueue = createQueue();
    p->time = clock();

    //Crea un mutex para ser compartido entre el Job y el CPU para accesar a la cola del ready
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    printf("Ojala me muera ya \n");
    //----------------------------------------------------------------
    /*
    PCB *pcb1 = createPCB(0,2,1); //PID, prority, state
    Process *process1 = createProcess(3,1,pcb1); //burst, arrival, pcb
    enqueue(process1,p->readyQueue);

    PCB *pcb2 = createPCB(1,2,1); //PID, prority, state
    Process *process2 = createProcess(2,2,pcb2); //burst, arrival, pcb
    enqueue(process2,p->readyQueue);

    PCB *pcb3 = createPCB(2,1,1); //PID, prority, state
    Process *process3 = createProcess(5,3,pcb3); //burst, arrival, pcb
    enqueue(process3,p->readyQueue);
    
    PCB *pcb4 = createPCB(3,3,1); //PID, prority, state
    Process *process4 = createProcess(4,3,pcb4); //burst, arrival, pcb
    enqueue(process4,p->readyQueue);
    */
    //FIN PRUEBAS-----------------------------------------------


    p->jobSchedulerThread = createJobScheduler(p->time,&mutex,p->readyQueue);
    //p->cpuSchedulerThread = createCPUScheduler(tipoAlgoritmo,quantum,&mutex,p->readyQueue);

    return p;

}

void insertData(int burst, int priority, Planner *p){
    //Insert_simple_queue(burst, priority, p->jobSchedulerThread->dataQueue);
    //printf("Inserted: %i %i \n",burst, priority);
    printf("Ojala me muera ya \n");
}




