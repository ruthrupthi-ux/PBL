#include <stdio.h>
#include <string.h>

#define SIZE 10

typedef struct
{
    int stack[SIZE];
    int top;
    int running;
} Stack;

void stack_init(Stack *s)
{
    s->top = -1;
    s->running = 1;
}

void execute_stack(Stack *s, char instruction[])
{
    char operation[10];
    int value;

    sscanf(instruction, "%s %d", operation, &value);

    if (strcmp(operation, "PUSH") == 0)
    {
        if (s->top < SIZE - 1)
        {
            s->stack[++s->top] = value;
            printf("PUSH: %d\n", value);
        }
        else
        {
            printf("Stack Overflow\n");
        }
    }

    else if (strcmp(operation, "POP") == 0)
    {
        if (s->top >= 0)
        {
            printf("POP: %d\n", s->stack[s->top--]);
        }
        else
        {
            printf("Stack Underflow\n");
        }
    }

    else if (strcmp(operation, "SPEEK") == 0)
    {
        if (s->top >= 0)
        {
            printf("Stack Top = %d\n", s->stack[s->top]);
        }
        else
        {
            printf("Stack is empty\n");
        }
    }

    else
    {
        printf("Invalid stack instruction\n");
    }
}