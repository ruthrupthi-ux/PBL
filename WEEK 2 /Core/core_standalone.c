#include <stdio.h>
#include <string.h>
#include "core.h"

static int execute(const char *instruction, CPU *cpu, Memory *memory, Stack *stack, Queue *queue, char *out, size_t n)
{
    char op[20];
    if (sscanf(instruction, "%19s", op) != 1) { snprintf(out,n,"Empty instruction"); return 0; }
    if (!strcmp(op,"LOAD") || !strcmp(op,"ADD") || !strcmp(op,"SUB") || !strcmp(op,"MUL") || !strcmp(op,"DIV") || !strcmp(op,"PRINT") || !strcmp(op,"HALT")) return cpu_execute(cpu,instruction,out,n);
    if (!strcmp(op,"STORE") || !strcmp(op,"READ")) return execute_memory(memory,instruction,out,n);
    if (!strcmp(op,"PUSH") || !strcmp(op,"POP") || !strcmp(op,"SPEEK")) return execute_stack(stack,instruction,out,n);
    if (!strcmp(op,"ENQUEUE") || !strcmp(op,"DEQUEUE") || !strcmp(op,"QPEEK")) return execute_queue(queue,instruction,out,n);
    snprintf(out,n,"Unknown instruction"); return 0;
}
int main(void)
{
    CPU cpu; Memory memory; Stack stack; Queue queue; char line[256], out[256];
    cpu_init(&cpu); memory_init(&memory); stack_init(&stack); queue_init(&queue);
    while (fgets(line,sizeof(line),stdin)) {
        line[strcspn(line,"\n")] = '\0';
        int ok=execute(line,&cpu,&memory,&stack,&queue,out,sizeof(out));
        printf("%s\n",out);
        if (!strcmp(line,"HALT")) break;
        (void)ok;
    }
    return 0;
}
