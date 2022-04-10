#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> 

void *revisarReadyQueue(void *data);

/*
Recibe los procesos a la cola del waiting, por lo que constantemente revisa si 
puede ejecutar
*/

typedef struct CPUSchedulerThread{
    pthread_t thread;
    pthread_mutex_t *mutex;
    Queue *readyQueue;
    Queue *finishedQueue;
    int tipoAlgoritmo;
    int quantum;
    int lastInTimeLine;
    long int time;
    //Me indica en cual segundo habia quedado el ultimo proceso que se ejecuto
    //o sea, por donde va el timeline.

}CPUSchedulerThread;

CPUSchedulerThread *createCPUScheduler(long int time,int tipoAlgoritmo,int quantum,pthread_mutex_t *mutex,Queue *readyQueue){

    CPUSchedulerThread *cpu = malloc(sizeof(CPUSchedulerThread));
    //Se crea el thread
    pthread_t thread;
    cpu->thread = thread;
    cpu->mutex = mutex;
    cpu->readyQueue = readyQueue;
    cpu->finishedQueue = createQueue();
    cpu->tipoAlgoritmo = tipoAlgoritmo;
    cpu->quantum = quantum;
    cpu->lastInTimeLine = 0;
    cpu->time = time;
    pthread_create(&thread,NULL,&revisarReadyQueue,(void*)cpu);
    return cpu;
}

//Declaracion de funciones 
void FIFO(CPUSchedulerThread *cpu);
void SJF(CPUSchedulerThread *cpu);
void HPF(CPUSchedulerThread *cpu);
void RR(CPUSchedulerThread *cpu);
void tables(CPUSchedulerThread *cpu);



void *revisarReadyQueue(void *data){
    
    CPUSchedulerThread *cpu = ((CPUSchedulerThread*)data);
    
    while(1){
        //Mientras haya algo en la cola del ready
        if (cpu->readyQueue->length>0){
            int tipo = cpu->tipoAlgoritmo;
            switch (tipo) {
                case 1:
                    FIFO(cpu);
                    break;
                case 2:
                    SJF(cpu);
                    break;
                case 3:
                    HPF(cpu);
                    break;
                case 4:
                    RR(cpu);
                    break;

                default:
                    break;
            }
        }
        else{
            break;
        }
        //Cuenta el tiempo de ocioso   
    }
    
    
}

/*
void tables(CPUSchedulerThread *cpu){


}
*/

/*
Esto lo hace bien si fuera un ejecicio como en clase y siempre el primero proceso entra en el segundo 0
No contempla continuar en la linea despues del ocio del CPU
*/
void FIFO(CPUSchedulerThread *cpu){

    //Bloqueo para que solo yo use la cola
    //pthread_mutex_lock(cpu->mutex);
    printf("--------------------JOB-------------------------\n");
    //Saco al primero de la cola
    Node *deleted = dequeue (cpu->readyQueue);
    //tomo su burst
    int burst = deleted->process->initialBurst;

    //Imprimo el burst para comprobar
    
    printf("PID/llegada: %i\n",deleted->process->pcb->PID);
    printf("Espera segun burst: %i\n",burst);

    //Espero cantidad de segundos segun burst
    usleep(burst*1000000); 
    //La salida se la asigno con clock
    int exit = clock()/CLOCKS_PER_SEC-cpu->time;
    deleted->process->exit = exit+1;

    //Imprimo la salida para comprobar
    printf("Salida: %i\n",exit+1);

    //Lo guardo en la cola de terminados
    enqueue (deleted->process,cpu->finishedQueue);
    //Desloqueo para que solo otro use la cola
    //pthread_mutex_unlock(cpu->mutex);
    
}


//Primero los procesos cuyo burst sea mas pequeño
void SJF(CPUSchedulerThread *cpu){

    //Bloqueo para que solo yo use la cola
    //pthread_mutex_lock(cpu->mutex);
    Node *shortestBurst = getShortest(cpu->readyQueue);
    int burst = shortestBurst->process->initialBurst;

    printf("--------------------JOB-------------------------\n");
    printf("PID/llegada: %i\n",shortestBurst->process->pcb->PID);
    printf("Espera segun menor burst: %i\n",burst);

    usleep(burst*1000000);
    int exit = (clock()/CLOCKS_PER_SEC-cpu->time);
    shortestBurst->process->exit = exit+1;

    //Imprimo la salida para comprobar
    printf("Salida: %i\n",exit+1);

    //Lo guardo en la cola de terminados
    enqueue (shortestBurst->process,cpu->finishedQueue);
    //Lo quito de la cola de en ready
    deleteNode(shortestBurst->process,cpu->readyQueue);
    //Desloqueo para que solo otro use la cola
    //pthread_mutex_unlock(cpu->mutex);



}

//Primero los procesos de mejor prioridad
void HPF(CPUSchedulerThread *cpu){

    //Bloqueo para que solo yo use la cola
    //pthread_mutex_lock(cpu->mutex);
    Node *betterPriotity = getBetterPriority(cpu->readyQueue);
    int burst = betterPriotity->process->initialBurst;

    printf("--------------------JOB-------------------------\n");
    printf("PID/llegada: %i\n",betterPriotity->process->pcb->PID);
    printf("Priority: %i\n",betterPriotity->process->pcb->priority);
    printf("Espera segun burst: %i\n",burst);

    usleep(burst*1000000); 
    //La salida se la asigno con clock
    int exit = (clock()/CLOCKS_PER_SEC-cpu->time);
    betterPriotity->process->exit = exit+1;


    //Imprimo la salida para comprobar
    printf("Salida: %i\n",exit+1);

    //Lo guardo en la cola de terminados
    enqueue (betterPriotity->process,cpu->finishedQueue);
    //Lo quito de la cola de en ready
    deleteNode(betterPriotity->process,cpu->readyQueue);
    //Desloqueo para que solo otro use la cola
    //pthread_mutex_unlock(cpu->mutex);


}


void RR(CPUSchedulerThread *cpu){
    
    //Bloqueo para que solo yo use la cola
    //pthread_mutex_lock(cpu->mutex);
    printf("--------------------JOB-------------------------\n");
    //Saco al primero de la cola
    Node *deleted = dequeue (cpu->readyQueue);
    //tomo su burst
    int burst = deleted->process->actualBurst;
    int quantum = cpu->quantum;
    
    if (burst > quantum){
        int restBurst = burst - quantum;
        deleted->process->actualBurst = restBurst;

        //Imprimo el PID para comprobar
        printf("PID/llegada: %i\n",deleted->process->pcb->PID);
        printf("Burst total: %i\n",deleted->process->initialBurst);
        printf("Espera segun quantum: %i\n",quantum);

        //Espero cantidad de segundos segun quantum
        usleep(quantum*1000000); 

        //La salida se la asigno con clock
        int exit = (clock()/CLOCKS_PER_SEC-cpu->time);
        deleted->process->exit = cpu->lastInTimeLine + exit + 1;

        //Imprimo la salida para comprobar
        printf("Salida: %i\n",exit+1);
        //Lo guardo en la cola de ready para que espere a ejecutarse por otro quantum
        enqueue (deleted->process,cpu->readyQueue);
    }
    else{
        //Imprimo el PID para comprobar
        printf("PID/llegada: %i\n",deleted->process->pcb->PID);
        printf("Burst total: %i\n",deleted->process->initialBurst);
        printf("Espera segun burst: %i\n",burst);

        //Espero cantidad de segundos segun burst
        usleep(burst*1000000); 

        //La salida se la asigno con clock
        int exit = (clock()/CLOCKS_PER_SEC-cpu->time);
        deleted->process->exit = cpu->lastInTimeLine + exit + 1;

        //Imprimo la salida para comprobar
        printf("Salida: %i\n",exit+1);
        //Lo guardo en la cola de terminados
        enqueue (deleted->process,cpu->finishedQueue);  
    }
    //Desloqueo para que solo otro use la cola
    //pthread_mutex_unlock(cpu->mutex);


}