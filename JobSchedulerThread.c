#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> 

void *revisarSimpleQueue(void *data);
//void showReady(JobSchedulerThread *job);



/*
Recibe los procesos del socket, les pone PID y los mete al ready
Siempre y cuando haya algo en ready lo va a poner en waiting
*/
typedef struct JobSchedulerThread{
    pthread_t thread;
    pthread_mutex_t *mutex;
    Simple_queue *dataQueue;
    Queue *readyQueue;
    int PIDCounter;
    long int time;

}JobSchedulerThread;

JobSchedulerThread *createJobScheduler(long int time, pthread_mutex_t *mutex, Queue *readyQueue){

    JobSchedulerThread *job = malloc(sizeof(JobSchedulerThread));
    //Se crea el thread
    pthread_t thread;
    job->thread = thread;
    job->mutex = mutex;
    job->readyQueue = readyQueue;
    job->PIDCounter = 0;
    job->time = time;
    job->dataQueue = createSimpleQueue();
    pthread_create(&thread,NULL,&revisarSimpleQueue,(void*)job);  
    pthread_join(thread, NULL);
    return job;
}


void *revisarSimpleQueue(void *data){
    /*
    JobSchedulerThread *job = ((JobSchedulerThread*)data);
    while(1){
        //Mientras haya algo en la cola del dataQueue
        if (job->dataQueue->length>0){
            SimpleNode *node = dequeueSimple(job->dataQueue);

            PCB *pcb = createPCB(job->PIDCounter,node->priority,1); //PID, prority, state
            job->PIDCounter = job->PIDCounter+1;
            //arrival hay que ponerselo segun el time
            Process *process = createProcess(node->burst,1,pcb); //burst, arrival, pcb

            pthread_mutex_lock(job->mutex);
            enqueue(process,job->readyQueue);
            pthread_mutex_unlock(job->mutex);

        }
        
    }
    */
  
    
    for(int i = 0; i<= 1; i++){
        usleep(5*100000);
        printf("JOB SCHEDULER\n");
    }

    
}