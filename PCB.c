#include <stdio.h>
#include <stdlib.h>

typedef struct PCB{
    int PID;
    int priority;
    int state;

}PCB;

PCB *createPCB(int pid,int priority, int state){
    PCB *pcb = malloc(sizeof(PCB));
    pcb->PID = pid;
    pcb->priority = priority;
    pcb->state = state;
    printf("PCB created!\n");
    return pcb;
}