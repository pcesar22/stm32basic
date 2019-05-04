#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdlib.h>
#include <stdio.h>

#define TRUE    1
#define FALSE	0

#define QUEUE_CMD_SIZE                    100

/* a link in the queue, holds the info and point to the next Node*/
typedef struct {
    char cmd[QUEUE_CMD_SIZE];
} DATA;

typedef struct Node_t {
    DATA data;
    struct Node_t *prev;
} NODE;

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
typedef struct Queue {
    NODE *head;
    NODE *tail;
    int size;
    int limit;
} Queue;

Queue *ConstructQueue(int limit);
void DestructQueue(Queue *queue);
int Enqueue(Queue *pQueue, NODE *item);
NODE *Dequeue(Queue *pQueue);
int isEmpty(Queue* pQueue);

#endif
