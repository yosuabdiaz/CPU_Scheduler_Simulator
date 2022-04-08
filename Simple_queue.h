#include <stdio.h>
#include <stdlib.h>

typedef struct SimpleNode {
    int burst;
    int priority;
    struct SimpleNode *next;
}SimpleNode;

//Constructor 
SimpleNode * createSimpleNode(int burst, int priority){
    SimpleNode * n = malloc(sizeof(SimpleNode));
    n->next = NULL;
    n->burst = burst;
    n->priority = priority;

}

typedef struct Simple_queue {
    SimpleNode *first;
    int length;

}Simple_queue;

//Constructor 
Simple_queue * createSimpleQueue(){
    Simple_queue * q = malloc(sizeof(Simple_queue));
    q->first = NULL;
    q->length = 0;
    return q;
}
