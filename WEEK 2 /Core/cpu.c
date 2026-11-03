#include <stdio.h>
#include <string.h>
#include "core.h"

void cpu_init(CPU *cpu)
{
    cpu->pc=0;
    cpu->accumulator=0;
    cpu->running=1;
}

int cpu_execute(CPU *cpu,const char *instruction,char *output,size_t output_size)
{
    char operation[20];
    int value;

    if (sscanf(instruction,"%19s",operation)!=1)
    {
        snprintf(output,output_size,"Invalid instruction");
        return 0;
    }

    if (!strcmp(operation,"LOAD") || !strcmp(operation,"ADD") || !strcmp(operation,"SUB") || !strcmp(operation,"MUL") || !strcmp(operation,"DIV"))
    {
        if (sscanf(instruction,"%19s %d",operation,&value)!=2)
        {
            snprintf(output,output_size,"Invalid CPU instruction");
            return 0;
        }

        if (!strcmp(operation,"LOAD"))
            cpu->accumulator=value;
        else if (!strcmp(operation,"ADD"))
            cpu->accumulator+=value;
        else if (!strcmp(operation,"SUB"))
            cpu->accumulator-=value;
        else if (!strcmp(operation,"MUL"))
            cpu->accumulator*=value;
        else
        {
            if (value==0)
            {
                snprintf(output,output_size,"Division by zero");
                return 0;
            }

            cpu->accumulator/=value;
        }
    }
    else if (!strcmp(operation,"PRINT"))
    {
        snprintf(output,output_size,"Accumulator = %d",cpu->accumulator);
    }
    else if (!strcmp(operation,"HALT"))
    {
        cpu->running=0;
        snprintf(output,output_size,"CPU stopped");
    }
    else
    {
        snprintf(output,output_size,"Unknown CPU instruction");
        return 0;
    }

    cpu->pc++;

    if (strcmp(operation,"PRINT")==0 || strcmp(operation,"HALT")==0)
        return 1;

    snprintf(output,output_size,"PC = %d, Accumulator = %d",cpu->pc,cpu->accumulator);

    return 1;
}
