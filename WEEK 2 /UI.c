#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
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
int write_full(int fd, const void *buffer, size_t size)
{
    const char *ptr = (const char *)buffer;
    size_t total = 0;
    while (total < size) {
        ssize_t n = write(fd, ptr + total, size - total);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (n == 0)
            return -1;
        total += (size_t)n;
    }
    return 0;
}
int read_full(int fd, void *buffer, size_t size)
{
    char *ptr = (char *)buffer;
    size_t total = 0;
    while (total < size) {
        ssize_t n = read(fd, ptr + total, size - total);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (n == 0)
            return -1;
        total += (size_t)n;
    }
    return 0;
}
int main(void)
{
    int to_core;
    int from_core;
    if (mkfifo(UI_TO_CORE_FIFO, 0666) == -1 &&
        errno != EEXIST) {
        perror("UI: FIFO creation failed");
        return 1;
    }
    if (mkfifo(CORE_TO_UI_FIFO, 0666) == -1 &&
        errno != EEXIST) {
        perror("UI: FIFO creation failed");
        return 1;
    }
    printf("\nWaiting for Core process...\n");
    to_core = open(UI_TO_CORE_FIFO, O_WRONLY);
    if (to_core == -1) {
        perror("UI: Unable to open UI-to-Core FIFO");
        return 1;
    }
    from_core = open(CORE_TO_UI_FIFO, O_RDONLY);
    if (from_core == -1) {
        perror("UI: Unable to open Core-to-UI FIFO");
        close(to_core);
        return 1;
    }
    int choice;
    int process_id = 0;
    int burst_time = 0;
    int priority = 0;
    char instruction[IPC_TEXT_SIZE];
    CoreResponse response;
    char latest_result[IPC_TEXT_SIZE] = "No instruction executed yet.";
    char process_status[32] = "READY";
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
            get_string(
                "Enter instruction (e.g., LOAD 10, ADD 5, PRINT): ",
                instruction,
                sizeof(instruction)
            );
            UIMessage msg = {0};
            msg.process_id = process_id;
            msg.command = 1;
            snprintf(msg.instruction, sizeof(msg.instruction), "%s", instruction);
            if (write_full(to_core, &msg, sizeof(msg)) == -1) {
                perror("UI: FIFO write failed");
                continue;
            }
            display_status(process_id, "RUNNING");
            printf("Request sent to Core through FIFO.\n");
            if (read_full(from_core, &response, sizeof(response)) == -1) {
                perror("UI: FIFO read failed");
                break;
            }
            snprintf(latest_result, sizeof(latest_result), "%s", response.message);
           if (response.status != 0) {
    snprintf(process_status, sizeof(process_status), "COMPLETED");
    display_status(process_id, "COMPLETED");
    display_result(process_id, response.message);
}
else {
    snprintf(process_status, sizeof(process_status), "ERROR");
    display_status(process_id, "ERROR");
    display_error(response.message);
}
}
else if (choice == 3) {
    if (process_id == 0)
        display_error("No process available.");
    else
        display_status(process_id, process_status);
}
else if (choice == 4) {
    if (process_id == 0)
        display_error("No process has been created.");
    else
        display_result(process_id, latest_result);
}
        else {
            display_error("Invalid choice. Please select 1-5.");
        }
    }
    close(to_core);
    close(from_core);
    return 0;
}
