#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mqueue.h>
#include <errno.h>
#include "ipc_common.h"

#define MAX_INPUT 100

int get_integer(const char *message)
{
    int value;
    printf("%s", message);
    while (scanf("%d", &value) != 1) {
        printf("Invalid input. Please enter a number: ");
        while (getchar() != '\n') {}
    }
    while (getchar() != '\n') {}
    return value;
}

void get_string(const char *message, char *buffer, int size)
{
    printf("%s", message);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

void display_menu(void)
{
    printf("\n========================================\n");
    printf("       MULTI-PROCESS SIMULATOR\n");
    printf("========================================\n");
    printf("1. Create Process\n");
    printf("2. Execute Process\n");
    printf("3. View Process Status\n");
    printf("4. View Execution Result\n");
    printf("5. Exit\n");
    printf("========================================\n");
}

void display_process(int id, int burst, int priority)
{
    printf("\n========================================\n");
    printf("          PROCESS INFORMATION\n");
    printf("========================================\n");
    printf("Process ID  : %d\n", id);
    printf("Burst Time  : %d\n", burst);
    printf("Priority    : %d\n", priority);
    printf("========================================\n");
}

void display_status(int id, const char *status)
{
    printf("\nProcess ID : %d\nStatus     : %s\n", id, status);
}

void display_result(int id, const char *result)
{
    printf("\nProcess ID : %d\nResult     : %s\n", id, result);
}

void display_error(const char *message)
{
    printf("\n[ERROR] %s\n", message);
}

void display_success(const char *message)
{
    printf("\n[SUCCESS] %s\n", message);
}

int main(void)
{
    mqd_t to_core = mq_open(UI_TO_CORE_QUEUE, O_WRONLY);
    mqd_t from_core = mq_open(CORE_TO_UI_QUEUE, O_RDONLY);

    if (to_core == (mqd_t)-1 || from_core == (mqd_t)-1) {
        perror("UI: unable to open IPC queues. Start with ./launcher");
        if (to_core != (mqd_t)-1) mq_close(to_core);
        if (from_core != (mqd_t)-1) mq_close(from_core);
        return 1;
    }

    int choice, process_id = 0, burst_time = 0, priority = 0;
    char instruction[IPC_TEXT_SIZE];
    CoreResponse response;

    printf("\n========================================\n");
    printf("     MULTI-PROCESS SIMULATOR UI\n");
    printf("          STUDENT 1 - UI\n");
    printf("========================================\n");

    while (1) {
        display_menu();
        choice = get_integer("Enter your choice: ");

        if (choice == 1) {
            process_id = get_integer("Enter Process ID: ");
            burst_time = get_integer("Enter Burst Time: ");
            priority = get_integer("Enter Priority: ");
            display_process(process_id, burst_time, priority);
            display_success("Process information captured.");
        }
        else if (choice == 2) {
            if (process_id == 0) {
                display_error("No process has been created yet.");
                continue;
            }

            get_string("Enter instruction (e.g., LOAD 10, ADD 5, PRINT): ",
                        instruction, sizeof(instruction));

            UIMessage msg = {0};
            msg.process_id = process_id;
            msg.burst_time = burst_time;
            msg.priority = priority;
            msg.command = 1;
            strncpy(msg.instruction, instruction, IPC_TEXT_SIZE - 1);

            if (mq_send(to_core, (const char *)&msg, sizeof(msg), 0) == -1) {
                perror("UI: mq_send");
                continue;
            }

            display_status(process_id, "RUNNING");
            printf("Request sent to Core through POSIX Message Queue.\n");

            if (mq_receive(from_core, (char *)&response, sizeof(response), NULL) == -1) {
                perror("UI: mq_receive");
                continue;
            }

            if (response.status == 0) {
                display_status(process_id, "COMPLETED");
                display_result(process_id, response.message);
            } else {
                display_status(process_id, "ERROR");
                display_error(response.message);
            }
        }
        else if (choice == 3) {
            if (process_id == 0)
                display_error("No process available.");
            else
                display_status(process_id, "READY");
        }
        else if (choice == 4) {
            if (process_id == 0)
                display_error("No process has been created.");
            else
                display_result(process_id, "Latest result is displayed after execution.");
        }
        else if (choice == 5) {
            UIMessage msg = {0};
            msg.command = 2;
            msg.process_id = process_id;
            mq_send(to_core, (const char *)&msg, sizeof(msg), 0);
            printf("\nExiting Multi-Process Simulator...\n");
            break;
        }
        else {
            display_error("Invalid choice. Please select 1-5.");
        }
    }

    mq_close(to_core);
    mq_close(from_core);
    return 0;
}
