#include <stdio.h>
#include <stdlib.h>
#include "Simple_queue.h"

void Insert_simple_queue(int burst, int priority, Simple_queue *q){
	if (q->length==0){
        q->first = createSimpleNode(burst, priority);
        q->length+=1;
        printf("Data enqueued!\n");
    }

    else 
    {
        //Referencia al primero
        SimpleNode* actual = q->first ;
        while (actual->next != NULL)
        	actual = actual->next; 
              
        SimpleNode* new = createSimpleNode(burst, priority);
        //Se le asigna al next del penunltimo
        actual->next = new;
        q->length+=1;
        printf("Data enqueued!\n");
    }
}



SimpleNode *dequeueSimple (Simple_queue *q){

    if (q->length==0){
        printf("There is nothing in the queue!\n");
		return NULL;
    }
    else{
        //Referencia al primero
        SimpleNode* deleted = q->first;    
        //El primero ahora es el segundo
		q->first = q->first->next;
        //Se desenlaza el nodo
		deleted->next = NULL;
        //Retorna el borrado
        printf("Data dequeued!\n");
        q->length-=1;
		return deleted;
    }

}


void printSimpleQueue(Simple_queue *q){

    SimpleNode *temp = q->first;

    while(temp!=NULL){
        //Imprime el burst para probar
        printf("Initial Burst: %i \n Priority: %i \n",temp->burst, temp->priority);
        //Avanza al siguiente nodo
        temp = temp->next;
    }

}