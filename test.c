// F6 Compiler
// gcc hello.c -pthread
#include <stdio.h>
#include <pthread.h>

void *thread_routine(void *arg){
    int nr_lines = *((int*)arg);
    printf("El hilo comienza a ejecutarse... \n");
    for(int i=0; i<nr_lines; i++){
        printf("tec... \n");        
    }
}

int main(int argc, char *argv[]){
    pthread_t thread1;
    int value = atoi(argv[1]);
    if (0 != pthread_create(&thread1, NULL, thread_routine, &value))
        return -1;
    pthread_join(thread1, NULL);
    return 0;  
}