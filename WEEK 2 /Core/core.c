#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mqueue.h>
#include "ipc_common.h"

#include "cpu.c"
#include "memory.c"
#include "stack.c"
#include "queue.c"

static void send_response(mqd_t ui_queue, int process_id, int status, const char *message)
{
    CoreResponse response = {0};
    response.process_id = process_id;
    response.status = status;
    strncpy(response.message, message, IPC_TEXT_SIZE - 1);

    if (mq_send(ui_queue, (const char *)&response, sizeof(response), 0) == -1)
        perror("CORE: unable to send response to UI");
}

static void send_log(mqd_t log_queue, int process_id, int level, const char *message)
{
    LogMessage log = {0};
    log.process_id = process_id;
    log.level = level;
    strncpy(log.message, message, IPC_TEXT_SIZE - 1);

    if (mq_send(log_queue, (const char *)&log, sizeof(log), 0) == -1)
        perror("CORE: unable to send log");
}

int main(void)
{
    CPU cpu;
    Memory mem;
    Stack stack;
    Queue queue;

    cpu_init(&cpu);
    memory_init(&mem);
    stack_init(&stack);
    queue_init(&queue);

    mqd_t from_ui = mq_open(UI_TO_CORE_QUEUE, O_RDONLY);
    mqd_t to_ui = mq_open(CORE_TO_UI_QUEUE, O_WRONLY);
    mqd_t to_log = mq_open(CORE_TO_LOG_QUEUE, O_WRONLY);

    if (from_ui == (mqd_t)-1 || to_ui == (mqd_t)-1 || to_log == (mqd_t)-1) {
        perror("CORE: unable to open IPC queues");
        return 1;
    }

    printf("CORE PROCESS STARTED\n");
    printf("Waiting for instructions from UI...\n");

    UIMessage msg;

    while (1) {
        if (mq_receive(from_ui, (char *)&msg, sizeof(msg), NULL) == -1) {
            perror("CORE: mq_receive");
            break;
        }

        if (msg.command == 2) {
            send_log(to_log, msg.process_id, 0, "Core process received shutdown request.");
            break;
        }

        char operation[20] = {0};
        if (sscanf(msg.instruction, "%19s", operation) != 1) {
            send_response(to_ui, msg.process_id, 1, "Empty instruction.");
            send_log(to_log, msg.process_id, 2, "Empty instruction received.");
            continue;
        }

        int known = 0;
        if (!strcmp(operation, "LOAD") || !strcmp(operation, "ADD") ||
            !strcmp(operation, "SUB") || !strcmp(operation, "MUL") ||
            !strcmp(operation, "DIV") || !strcmp(operation, "PRINT")) {
            known = 1;
            execute(&cpu, msg.instruction);
        }
        else if (!strcmp(operation, "STORE") || !strcmp(operation, "READ")) {
            known = 1;
            execute_memory(&mem, msg.instruction);
        }
        else if (!strcmp(operation, "PUSH") || !strcmp(operation, "POP") ||
                 !strcmp(operation, "SPEEK")) {
            known = 1;
            execute_stack(&stack, msg.instruction);
        }
        else if (!strcmp(operation, "ENQUEUE") || !strcmp(operation, "DEQUEUE") ||
                 !strcmp(operation, "QPEEK")) {
            known = 1;
            execute_queue(&queue, msg.instruction);
        }
        else if (!strcmp(operation, "HALT")) {
            known = 1;
            printf("CORE PROCESS HALTED\n");
            send_response(to_ui, msg.process_id, 0, "HALT received. Core is stopping.");
            send_log(to_log, msg.process_id, 0, "HALT instruction received.");
            send_log(to_log, msg.process_id, 0, "Core shutdown request.");
            break;
        }

        if (known) {
            char result[IPC_TEXT_SIZE];
            snprintf(result, sizeof(result), "Instruction executed: %s", msg.instruction);
            send_response(to_ui, msg.process_id, 0, result);
            send_log(to_log, msg.process_id, 0, result);
        } else {
            send_response(to_ui, msg.process_id, 1, "Invalid instruction.");
            send_log(to_log, msg.process_id, 2, "Invalid instruction received.");
        }
    }

    mq_close(from_ui);
    mq_close(to_ui);
    mq_close(to_log);
    return 0;
}
