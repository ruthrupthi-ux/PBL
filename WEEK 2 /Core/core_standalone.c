#include <stdio.h>
#include <string.h>

#include "cpu.c"
#include "memory.c"
#include "stack.c"
#include "queue.c"

int main()
{
    CPU cpu;
    Memory mem;
    Stack stack;
    Queue queue;

    char instruction[50];
    char operation[20];

    cpu_init(&cpu);
    memory_init(&mem);
    stack_init(&stack);
    queue_init(&queue);

    while (1)
    {
        printf("\nEnter instruction: ");

        if (fgets(instruction, sizeof(instruction), stdin) == NULL)
            break;

        instruction[strcspn(instruction, "\n")] = '\0';

        sscanf(instruction, "%s", operation);

        if (strcmp(operation, "LOAD") == 0 ||
            strcmp(operation, "ADD") == 0 ||
            strcmp(operation, "SUB") == 0 ||
            strcmp(operation, "MUL") == 0 ||
            strcmp(operation, "DIV") == 0 ||
            strcmp(operation, "PRINT") == 0)
        {
            execute(&cpu, instruction);
        }
        else if (strcmp(operation, "STORE") == 0 ||
                 strcmp(operation, "READ") == 0)
        {
            execute_memory(&mem, instruction);
        }
        else if (strcmp(operation, "PUSH") == 0 ||
                 strcmp(operation, "POP") == 0 ||
                 strcmp(operation, "SPEEK") == 0)
        {
            execute_stack(&stack, instruction);
        }
        else if (strcmp(operation, "ENQUEUE") == 0 ||
                 strcmp(operation, "DEQUEUE") == 0 ||
                 strcmp(operation, "QPEEK") == 0)
        {
            execute_queue(&queue, instruction);
        }
        else if (strcmp(operation, "HALT") == 0)
        {
            printf("\nCORE PROCESS HALTED\n");
            break;
        }
        else
        {
            printf("Invalid instruction\n");
        }
    }

    return 0;
}
