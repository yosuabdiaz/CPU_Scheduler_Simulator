#include <stdio.h>
#include "Process.c"

int main(void){

    PCB *pcb = createPCB(1,1,1);
    Process *process = createProcess(3,0,pcb);
}