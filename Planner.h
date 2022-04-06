#include <stdio.h>
#include <pthread.h>



typedef struct Planner {
    JobSchedulerThread *jobSchedulerThread;
    CPUSchedulerThread *cpuSchedulerThread;
    int tipoAlgoritmo;
    Queue *waitingQueue;
    

} Planner;

//Constructor 
Planner * createPlanner(int tipoAlgoritmo){   
    Planner *p = malloc(sizeof(Planner));
    p->tipoAlgoritmo = tipoAlgoritmo;
    p->waitingQueue = createQueue();
    //Crea un mutex para ser compartido entre el Job y el CPU 
    //Para accesar a la cola del waiting
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    //ALGUNOS PROCESOS SE AGREGAN A LA COLA PARA PROBAR-----------------------------
    PCB *pcb1 = createPCB(1,2,1); //PID, prority, state
    Process *process1 = createProcess(3,1,pcb1); //burst, arrival, pcb
    enqueue(process1,p->waitingQueue);

    PCB *pcb2 = createPCB(2,2,1); //PID, prority, state
    Process *process2 = createProcess(2,2,pcb2); //burst, arrival, pcb
    enqueue(process2,p->waitingQueue);

    PCB *pcb3 = createPCB(3,1,1); //PID, prority, state
    Process *process3 = createProcess(5,3,pcb3); //burst, arrival, pcb
    enqueue(process3,p->waitingQueue);

    PCB *pcb4 = createPCB(4,3,1); //PID, prority, state
    Process *process4 = createProcess(4,3,pcb4); //burst, arrival, pcb
    enqueue(process4,p->waitingQueue);

    //FIN PRUEBAS-----------------------------------------------

    p->jobSchedulerThread = createJobScheduler(&mutex,p->waitingQueue );
    p->cpuSchedulerThread = createCPUScheduler(&mutex,p->waitingQueue );

    return p;
}

