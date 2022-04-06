#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *printThreadcpu(void *data);

/*
Recibe los procesos a la cola del waiting, por lo que constantemente revisa si 
puede ejecutar
*/

typedef struct CPUSchedulerThread{
    pthread_t thread;
    pthread_mutex_t *mutex;
    Queue *waiting;

}CPUSchedulerThread;

CPUSchedulerThread *createCPUScheduler(pthread_mutex_t *mutex,Queue *waiting){

    CPUSchedulerThread *cpu = malloc(sizeof(CPUSchedulerThread));
    //Se crea el thread
    pthread_t thread;
    cpu->thread = thread;
    cpu->mutex = mutex;
    cpu->waiting = waiting;
    pthread_create(&thread,NULL,&printThreadcpu,"PRUEBA CPU");
    pthread_join(thread,NULL);
}

void *printThreadcpu(void *data){
    //Aqui lo que haga el hilo
    printf("%s\n",(char*)data);
}
