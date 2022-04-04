#include <stdio.h>
#include "Queue.c"
#include "JobSchedulerThread.c"
#include "CPUSchedulerThread.c"

int main(void){
    /*
    PCB *pcb = createPCB(1,2,1); //PID, prority, state
    Process *process = createProcess(3,0,pcb); //burst, arrival, pcb
    Queue *q = createQueue();
    enqueue(process,q);
    //dequeue(q);
    //dequeue(q);
    printQueue(q); */
    JobSchedulerThread *jobScheduler = createJobScheduler();
    CPUSchedulerThread *cpuScheduler = createCPUScheduler();
}