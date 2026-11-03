#ifndef CORE_H
#define CORE_H
#include <stddef.h>
#define SIZE 10
typedef struct { int pc; int accumulator; int running; } CPU;
typedef struct { int memory[SIZE]; int running; } Memory;
typedef struct { int stack[SIZE]; int top; int running; } Stack;
typedef struct { int queue[SIZE]; int front; int rear; int running; } Queue;
void cpu_init(CPU *cpu);
int cpu_execute(CPU *cpu, const char *instruction, char *output, size_t output_size);
void memory_init(Memory *memory);
int execute_memory(Memory *memory, const char *instruction, char *output, size_t output_size);
void stack_init(Stack *stack);
int execute_stack(Stack *stack, const char *instruction, char *output, size_t output_size);
void queue_init(Queue *queue);
int execute_queue(Queue *queue, const char *instruction, char *output, size_t output_size);
int run_core(int ui_read_fd, int ui_write_fd, int logger_write_fd);
#endif
