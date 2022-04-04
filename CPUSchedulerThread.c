#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *printThreadcpu(void *data);

typedef struct CPUSchedulerThread{
    pthread_t thread;
    Queue *queue;
    //mutex

}CPUSchedulerThread;

CPUSchedulerThread *createCPUScheduler(){

    CPUSchedulerThread *cpu = malloc(sizeof(CPUSchedulerThread));
    pthread_t thread;
    cpu->thread = thread;
    pthread_create(&thread,NULL,&printThreadcpu,"PRUEBA CPU");
    pthread_join(thread,NULL);
}

void *printThreadcpu(void *data){
    //Aqui lo que haga el hilo
    printf("%s",(char*)data);
}
