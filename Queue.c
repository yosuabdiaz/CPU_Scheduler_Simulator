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