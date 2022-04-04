#include <stdio.h>


typedef struct Planner {
    JobSchedulerThread *jobSchedulerThread;
    CPUSchedulerThread *cpuSchedulerThread;
    Queue *queue;
    int tipoAlgoritmo;

} Planner;

//Constructor 
Planner * createPlanner(int tipoAlgoritmo, Queue *queue){   //
    Planner *p = malloc(sizeof(Planner));
    p->queue = queue;
    p->tipoAlgoritmo = tipoAlgoritmo;
    p->jobSchedulerThread = createJobScheduler();
    p->cpuSchedulerThread = createCPUScheduler();

    return p;
}
