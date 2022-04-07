#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *printThread(void *data);

//void showReady(JobSchedulerThread *job);



/*
Recibe los procesos del socket, les pone PID y los mete al ready
Siempre y cuando haya algo en ready lo va a poner en waiting
*/
typedef struct JobSchedulerThread{
    pthread_t thread;
    pthread_mutex_t *mutex;
    //Queue *mientras; Otro tipo de cola para el string burst,prioridad
    Queue *readyQueue;

}JobSchedulerThread;

JobSchedulerThread *createJobScheduler(pthread_mutex_t *mutex, Queue *readyQueue){

    JobSchedulerThread *job = malloc(sizeof(JobSchedulerThread));
    //Se crea el thread
    pthread_t thread;
    job->thread = thread;
    job->mutex = mutex;
    job->readyQueue = readyQueue;
    pthread_create(&thread,NULL,&printThread,"PRUEBA JOB");  //Aqui le paso lo que quiera -- (void*)job
    pthread_join(thread,NULL);
}

void *printThread(void *data){
    //Aqui lo que haga el hilo
    //int hi = ((JobSchedulerThread*)data)->hola;
    //aqui while true pasa al waiting los procesos 
    for (int i = 0; i < 1; i++ ){
        printf("%s\n",(char*)data);
    }
   
    
}
