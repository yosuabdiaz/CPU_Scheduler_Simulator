#include <stdio.h>
#include <stdlib.h>
#include "PCB.c"

typedef struct Process{
    int initialBurst;
    int actualBurst;
    int arrival;
    PCB *pcb;

}Process;

Process *createProcess(int burst,int arrival, PCB *pcb){
    Process *p = malloc(sizeof(Process));
    p->arrival = arrival;
    p->initialBurst = burst;
    p->actualBurst = burst;
    printf("Process created!\n");
    return p;
}