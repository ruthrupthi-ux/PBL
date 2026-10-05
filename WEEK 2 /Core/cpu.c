#include <stdio.h>
#include <string.h>

typedef struct
{
    int pc;
    int accumulator;
    int running;
} CPU;

void cpu_init(CPU *cpu)
{
    cpu->pc = 0;
    cpu->accumulator = 0;
    cpu->running = 1;
}

void execute(CPU *cpu, char instruction[])
{
    char operation[10];
    int value = 0;

    sscanf(instruction, "%s %d", operation, &value);

    printf("PC = %d\n", cpu->pc);

    if (strcmp(operation, "LOAD") == 0)
        cpu->accumulator = value;

    else if (strcmp(operation, "ADD") == 0)
        cpu->accumulator += value;

    else if (strcmp(operation, "SUB") == 0)
        cpu->accumulator -= value;

    else if (strcmp(operation, "MUL") == 0)
        cpu->accumulator *= value;

    else if (strcmp(operation, "DIV") == 0)
    {
        if (value == 0)
        {
            printf("Cannot divide by zero\n");
            return;
        }

        cpu->accumulator /= value;
    }

    else if (strcmp(operation, "PRINT") == 0)
        printf("Accumulator = %d\n", cpu->accumulator);

    else
    {
        printf("Invalid CPU instruction\n");
        return;
    }

    if (strcmp(operation, "PRINT") != 0)
        printf("Accumulator = %d\n", cpu->accumulator);

    cpu->pc++;
}