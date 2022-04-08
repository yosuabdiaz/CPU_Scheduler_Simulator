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
        dequeue (q);
    }
    else{
        while (actual->next->process != p){ 
            actual = actual->next; 
        }
        Node *temp = actual->next;
        actual->next = actual->next->next;
        temp->next = NULL;
        printf("Process deleted!\n");
    }
    return actual;       

}

Node *getHigher(Queue *q){

    if (q->length==0){ return NULL;}
    Node *temp = q->first;
    Node *higher = q->first;
    while(temp!=NULL){
        if (temp->process->initialBurst > higher->process->initialBurst){
            higher = temp;
        }
        temp = temp->next;
    }
    return higher;

}

void printQueue(Queue *q){

    Node *temp = q->first;

    while(temp!=NULL){
        //Imprime el burst para probar
        printf("Initial Burst: %i \n",temp->process->initialBurst);
        //Avanza al siguiente nodo
        temp = temp->next;
    }

}