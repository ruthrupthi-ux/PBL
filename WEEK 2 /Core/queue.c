#include <stdio.h>
#include <string.h>
#include "core.h"

void queue_init(Queue *queue)
{
    queue->front=0;
    queue->rear=-1;
    queue->running=1;
}

int execute_queue(Queue *queue,const char *instruction,char *output,size_t output_size)
{
    char operation[20];
    int value;

    if(sscanf(instruction,"%19s",operation)!=1)
    {
        snprintf(output,output_size,"Empty instruction");
        return 0;
    }

    if(!strcmp(operation,"ENQUEUE"))
    {
        if(sscanf(instruction,"%19s %d",operation,&value)!=2)
        {
            snprintf(output,output_size,"Invalid ENQUEUE instruction");
            return 0;
        }

        if(queue->rear>=SIZE-1)
        {
            snprintf(output,output_size,"Queue overflow");
            return 0;
        }

        queue->queue[++queue->rear]=value;
        snprintf(output,output_size,"Enqueued %d",value);
    }
    else if(!strcmp(operation,"DEQUEUE"))
    {
        if(queue->front>queue->rear)
        {
            snprintf(output,output_size,"Queue is empty");
            return 0;
        }

        value=queue->queue[queue->front++];
        snprintf(output,output_size,"Dequeued %d",value);

        if(queue->front>queue->rear)
        {
            queue->front=0;
            queue->rear=-1;
        }
    }
    else if(!strcmp(operation,"QPEEK"))
    {
        if(queue->front>queue->rear)
        {
            snprintf(output,output_size,"Queue is empty");
            return 0;
        }

        snprintf(output,output_size,"Front element = %d",queue->queue[queue->front]);
    }
    else
    {
        snprintf(output,output_size,"Unknown queue instruction");
        return 0;
    }

    return 1;
}


