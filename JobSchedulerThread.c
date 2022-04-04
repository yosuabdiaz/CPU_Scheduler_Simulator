#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *printThread(void *data);

typedef struct JobSchedulerThread{
    pthread_t thread;
    Queue *queue;
    //mutex

}JobSchedulerThread;

JobSchedulerThread *createJobScheduler(){

    JobSchedulerThread *job = malloc(sizeof(JobSchedulerThread));
    pthread_t thread;
    job->thread = thread;
    pthread_create(&thread,NULL,&printThread,"PRUEBA JOB");
    pthread_join(thread,NULL);
}

void *printThread(void *data){
    //Aqui lo que haga el hilo
    printf("%s",(char*)data);
}
