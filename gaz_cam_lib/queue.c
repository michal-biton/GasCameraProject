#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#include"queue.h"


int enqueue(Queue* queue,Node* node)
{
    sem_wait(&queue->sema);
    if(queue->counter==queue->capacity)
    {
        sem_post(&queue->sema);
        return 1;
    }
    else
    {
    if(queue->head==NULL)
        (queue)->head=node;
    else
        queue->tail->next=node;
    queue->counter++;
    queue->tail=node;
    }
    sem_post(&queue->sema);
     return 0;
}

Node* dequeue(Queue* queue)
{
    Node* tempNode=NULL;
    sem_wait(&queue->sema);
     if(!isEmpty(queue)){
        queue->counter--;
        tempNode=(queue)->head;
        queue->head=queue->head->next;
     }
     sem_post(&queue->sema);
     return tempNode;
}

Node* createNode(void* data)
{
    Node* node=(Node*)malloc(sizeof(Node));
    assert(node);
    node->data=data;
    node->next=NULL;
    return node;
}

Queue* createQueue(int max)
{
    Queue* queue=(Queue*)malloc(sizeof(Queue));
    assert(queue);
   queue->capacity=max;
    queue->counter=0;
    queue->head=NULL;
    queue->tail=NULL;
    sem_init(&queue->sema,0,1);
    return queue;
}

void freeNode(Node* node)
{
    if(!node)
        free(node);
    node=0;
}

void freeQueue(Queue* queue)
{
    if(queue){
    while(queue->head)
        freeNode(dequeue(queue));
    free(queue);
    queue=0;
    }
}

int isEmpty(Queue* queue){
    return !(queue->counter);
}

Node* top(Queue* queue){
    Node* tempNode=NULL;
    sem_wait(&queue->sema);
     if(!isEmpty(queue)){
        tempNode=queue->head;
     }
     return tempNode;
}

