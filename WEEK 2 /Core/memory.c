#include <stdio.h>
#include <string.h>

#define SIZE 10

typedef struct
{
    int memory[SIZE];
    int running;
} Memory;

void memory_init(Memory *mem)
{
    int i;

    for (i = 0; i < SIZE; i++)
        mem->memory[i] = 0;

    mem->running = 1;
}

void execute_memory(Memory *mem, char instruction[])
{
    char operation[10];
    int address, value;

    printf("FETCH: %s\n", instruction);

    sscanf(instruction, "%s %d %d",
           operation, &address, &value);

    if (strcmp(operation, "STORE") == 0)
    {
        if (address >= 0 && address < SIZE)
        {
            mem->memory[address] = value;
            printf("Memory[%d] = %d\n", address, value);
        }
        else
            printf("Invalid memory address\n");
    }

    else if (strcmp(operation, "READ") == 0)
    {
        if (address >= 0 && address < SIZE)
            printf("Memory[%d] = %d\n",
                   address, mem->memory[address]);
        else
            printf("Invalid memory address\n");
    }

    else
        printf("Invalid memory instruction\n");
}