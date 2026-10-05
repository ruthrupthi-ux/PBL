#include <stdio.h>
#include <string.h>

#define SIZE 10

typedef struct
{
    int queue[SIZE];
    int front;
    int rear;
    int running;
} Queue;

void queue_init(Queue *q)
{
    q->front = 0;
    q->rear = -1;
    q->running = 1;
}

void execute_queue(Queue *q, char instruction[])
{
    char operation[10];
    int value;

    sscanf(instruction, "%s %d", operation, &value);

    if (strcmp(operation, "ENQUEUE") == 0)
    {
        if (q->rear < SIZE - 1)
        {
            q->rear++;
            q->queue[q->rear] = value;

            printf("ENQUEUE: %d\n", value);
        }
        else
        {
            printf("Queue Overflow\n");
        }
    }

    else if (strcmp(operation, "DEQUEUE") == 0)
    {
        if (q->front <= q->rear)
        {
            printf("DEQUEUE: %d\n", q->queue[q->front]);
            q->front++;
        }
        else
        {
            printf("Queue Underflow\n");
        }
    }

    else if (strcmp(operation, "QPEEK") == 0)
    {
        if (q->front <= q->rear)
        {
            printf("Queue Front = %d\n", q->queue[q->front]);
        }
        else
        {
            printf("Queue is empty\n");
        }
    }

    else
    {
        printf("Invalid queue instruction\n");
    }
}