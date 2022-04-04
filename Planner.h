#include <stdio.h>
#include "Queue.c"
#include "JobSchedulerThread.c"
#include "CPUSchedulerThread.c"

typedef struct Planner {
    JobSchedulerThread *jobSchedulerThread;
    CPUSchedulerThread *cpuSchedulerThread;
    Queue *queue;

} Planner;

//Constructor 
Planner * createPlanner(){
    Planner *p = malloc(sizeof(Planner));
    p->jobSchedulerThread = createJobScheduler();;
    p->cpuSchedulerThread = createCPUScheduler();
    return p;
}
