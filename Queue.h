#include <stdio.h>
#include <stdlib.h>
#include "Process.c"

typedef struct Node {
    Process *process;
    struct Node *next;

}Node;

//Constructor 
Node * createNode(Process *process){
    Node * n = malloc(sizeof(Node));
    n->next = NULL;
    n->process = process;

}

typedef struct Queue {
    Node *first;
    int length;

}Queue;

//Constructor 
Queue * createQueue(){
    Queue * q = malloc(sizeof(Queue));
    q->first = NULL;
    q->length = 0;
    return q;
}
