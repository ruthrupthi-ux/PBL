
#include <stdio.h>
#include <string.h>
#include "core.h"

void memory_init(Memory *memory)
{
    for(int i=0;i<SIZE;i++)
        memory->memory[i]=0;

    memory->running=1;
}

int execute_memory(Memory *memory,const char *instruction,char *output,size_t output_size)
{
    char operation[20];
    int index,value;

    if(sscanf(instruction,"%19s",operation)!=1)
    {
        snprintf(output,output_size,"Empty instruction");
        return 0;
    }

    if(!strcmp(operation,"STORE"))
    {
        if(sscanf(instruction,"%19s %d %d",operation,&index,&value)!=3)
        {
            snprintf(output,output_size,"Invalid STORE instruction");
            return 0;
        }

        if(index<0 || index>=SIZE)
        {
            snprintf(output,output_size,"Memory index out of range");
            return 0;
        }

        memory->memory[index]=value;
        snprintf(output,output_size,"Stored %d at memory[%d]",value,index);
    }
    else if(!strcmp(operation,"READ"))
    {
        if(sscanf(instruction,"%19s %d",operation,&index)!=2)
        {
            snprintf(output,output_size,"Invalid READ instruction");
            return 0;
        }

        if(index<0 || index>=SIZE)
        {
            snprintf(output,output_size,"Memory index out of range");
            return 0;
        }

        snprintf(output,output_size,"memory[%d] = %d",index,memory->memory[index]);
    }
    else
    {
        snprintf(output,output_size,"Unknown memory instruction");
        return 0;
    }

    return 1;
}
