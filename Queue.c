#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

void enqueue (Process *p, Queue *q){
	if (q->length==0){
        q->first = createNode(p);
        q->length+=1;
        printf("Process enqueued!\n");
    }

    else 
    {
        //Referencia al primero
        Node* actual = q->first ;
        while (actual->next != NULL)
        	actual = actual->next; 
              
        Node* new = createNode(p);
        //Se le asigna al next del penunltimo
        actual->next = new;
        q->length+=1;
        printf("Process enqueued!\n");
    }
}



Node *dequeue (Queue *q){

    if (q->length==0){
        printf("There is nothing in the queue!\n");
		return NULL;
    }
    else{
        //Referencia al primero
        Node* deleted = q->first;    
        //El primero ahora es el segundo
		q->first = q->first->next;
        //Se desenlaza el nodo
		deleted->next = NULL;
        //Retorna el borrado
        printf("Process dequeued!\n");
        q->length-=1;
		return deleted;
    }
 

}

Node *deleteNode(Process *p,Queue *q){
    //Dado un proceso retorna el nodo en el que se encuentra
    Node* actual = q->first ;

    if (actual->process == p){
        return dequeue (q);
    }
    else{
        while (actual->next->process != p){ 
            actual = actual->next; 
        }
        //Nodo que ando buscando
        Node *temp = actual->next;
        actual->next = temp->next;
        temp->next = NULL;
        q->length-=1;
        printf("Process deleted!\n");
        return temp; 
    }       

}


Node *getBetterPriority(Queue *q){

    if (q->length==0){ return NULL;}
    Node *temp = q->first;
    Node *betterPriotity = q->first;
    while(temp!=NULL){
        if (temp->process->pcb->priority < betterPriotity->process->pcb->priority){
            betterPriotity = temp;
        }
        temp = temp->next;
    }
    return betterPriotity;

}



Node *getShortest(Queue *q){

    if (q->length==0){ return NULL;}
    Node *temp = q->first;
    Node *shortest = q->first;
    while(temp!=NULL){
        if (temp->process->initialBurst < shortest->process->initialBurst){
            shortest = temp;
        }
        temp = temp->next;
    }
    return shortest;

}

void printQueue(Queue *q){
    Node *temp = q->first;

    while(temp!=NULL){
        //Imprime el burst para probar
        printf("-----------------------------------------------------\n");
        printf("PID: %i Priority: %i \n",temp->process->pcb->PID,temp->process->pcb->priority);
        printf("Burst: %i Arrival: %i Exit: %i\n",temp->process->initialBurst,temp->process->arrival,temp->process->exit);
        printf("-----------------------------------------------------\n");
        //Avanza al siguiente nodo
        temp = temp->next;
    }

}

void printTable(Queue *q){
    Node *temp = q->first;
    printf("PID     TAT     WT\n");
    while(temp!=NULL){
        //Imprime el burst para probar
        printf("-------------------------------------\n");
        int TAT = temp->process->exit - temp->process->arrival;
        int WT = TAT - temp->process->initialBurst;
        printf("%i      %i      %i\n",temp->process->pcb->PID,TAT,WT);
        //Avanza al siguiente nodo
        temp = temp->next;
    }

}

void meanTAT(Queue *q){
    Node *temp = q->first;
    int mean = 0;
    printf("-------------------------------------\n");
    while(temp!=NULL){
        int TAT = temp->process->exit - temp->process->arrival;
        mean = mean + TAT;
        temp = temp->next;
    }
    printf("Mean TAT %i\n",mean/q->length);

}

void meanWT(Queue *q){
    Node *temp = q->first;
    int mean = 0;
    printf("-------------------------------------\n");
    while(temp!=NULL){
        int TAT = temp->process->exit - temp->process->arrival;
        int WT = TAT - temp->process->initialBurst;
        mean = mean + WT;
        temp = temp->next;
    }
    printf("Mean WT %i\n",mean/q->length);
}