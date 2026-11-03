
#include <stdio.h>
#include <string.h>
#include "core.h"

void stack_init(Stack *stack)
{
    stack->top=-1;
    stack->running=1;
}

int execute_stack(Stack *stack,const char *instruction,char *output,size_t output_size)
{
    char operation[20];
    int value;

    if(sscanf(instruction,"%19s",operation)!=1)
    {
        snprintf(output,output_size,"Empty instruction");
        return 0;
    }

    if(!strcmp(operation,"PUSH"))
    {
        if(sscanf(instruction,"%19s %d",operation,&value)!=2)
        {
            snprintf(output,output_size,"Invalid PUSH instruction");
            return 0;
        }

        if(stack->top>=SIZE-1)
        {
            snprintf(output,output_size,"Stack overflow");
            return 0;
        }

        stack->stack[++stack->top]=value;
        snprintf(output,output_size,"Pushed %d",value);
    }
    else if(!strcmp(operation,"POP"))
    {
        if(stack->top<0)
        {
            snprintf(output,output_size,"Stack underflow");
            return 0;
        }

        value=stack->stack[stack->top--];
        snprintf(output,output_size,"Popped %d",value);
    }
    else if(!strcmp(operation,"SPEEK"))
    {
        if(stack->top<0)
        {
            snprintf(output,output_size,"Stack is empty");
            return 0;
        }

        snprintf(output,output_size,"Top element = %d",stack->stack[stack->top]);
    }
    else
    {
        snprintf(output,output_size,"Unknown stack instruction");
        return 0;
    }

    return 1;
}
