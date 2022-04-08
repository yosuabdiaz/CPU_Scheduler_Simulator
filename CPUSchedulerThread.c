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
    int lastInTimeLine; 
    //Me indica en cual segundo habia quedado el ultimo proceso que se ejecuto
    //o sea, por donde va el timeline.

}CPUSchedulerThread;

CPUSchedulerThread *createCPUScheduler(int tipoAlgoritmo,pthread_mutex_t *mutex,Queue *readyQueue){

    CPUSchedulerThread *cpu = malloc(sizeof(CPUSchedulerThread));
    //Se crea el thread
    pthread_t thread;
    cpu->thread = thread;
    cpu->mutex = mutex;
    cpu->readyQueue = readyQueue;
    cpu->finishedQueue = createQueue();
    cpu->tipoAlgoritmo = tipoAlgoritmo;
    cpu->lastInTimeLine = 0;
    pthread_create(&thread,NULL,&revisarReadyQueue,(void*)cpu);
    pthread_join(thread,NULL);
}

//Declaracion de funciones 
void FIFO(CPUSchedulerThread *cpu);
void SJF(CPUSchedulerThread *cpu);
void HPF(CPUSchedulerThread *cpu);
void RR(CPUSchedulerThread *cpu);


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

            }
            
        }
        //Cuenta el tiempo de ocioso
        
    }
    
    
}

/*
Esto lo hace bien si fuera un ejecicio como en clase y siempre el primero proceso entra en el segundo 0
No contempla continuar en la linea despues del ocio del CPU
*/
void FIFO(CPUSchedulerThread *cpu){

    //Bloqueo para que solo yo use la cola
    pthread_mutex_lock(cpu->mutex);
    //Saco al primero de la cola
    Node *deleted = dequeue (cpu->readyQueue);
    //tomo su burst
    int burst = deleted->process->initialBurst;

    //Imprimo el burst para comprobar
    printf("---------------------------------------------\n");
    printf("PID/llegada: %i\n",deleted->process->pcb->PID);
    printf("Espera segun burst: %i\n",burst);

    //Espero cantidad de segundos segun burst
    usleep(burst*1000000); 
    //La salida se la asigno como tiempo del proceso anterior en salir mas mi burst
    deleted->process->exit = cpu->lastInTimeLine + burst;
    cpu->lastInTimeLine = cpu->lastInTimeLine + burst;

    //Imprimo la salida para comprobar
    printf("Salida: %i\n",deleted->process->exit);

    //Lo guardo en la cola de terminados
    enqueue (deleted->process,cpu->finishedQueue);
    //Desloqueo para que solo otro use la cola
    pthread_mutex_unlock(cpu->mutex);
}


//Estan mal los tiempos
void SJF(CPUSchedulerThread *cpu){

    //Bloqueo para que solo yo use la cola
    pthread_mutex_lock(cpu->mutex);
    Node *higherBurst = getHigher(cpu->readyQueue);
    int burst = higherBurst->process->initialBurst;

    printf("---------------------------------------------\n");
    printf("PID/llegada: %i\n",higherBurst->process->pcb->PID);
    printf("Espera segun burst: %i\n",burst);

    usleep(burst*1000000); 
    deleteNode(higherBurst->process,cpu->readyQueue);
    //La salida se la asigno como tiempo del proceso anterior en salir mas mi burst
    higherBurst->process->exit = cpu->lastInTimeLine + burst;
    cpu->lastInTimeLine = cpu->lastInTimeLine + burst;

    //Imprimo la salida para comprobar
    printf("Salida: %i\n",higherBurst->process->exit);

    //Lo guardo en la cola de terminados
    enqueue (higherBurst->process,cpu->finishedQueue);
    //Desloqueo para que solo otro use la cola
    pthread_mutex_unlock(cpu->mutex);


}