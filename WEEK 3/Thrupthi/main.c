
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

static int read_int(const char *prompt, int min, int max)
{
    char line[128];
    char extra;
    int value;

    while (1) {
        printf("%s", prompt);
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\nInput ended. Exiting.\n");
            exit(EXIT_FAILURE);
        }

        if (sscanf(line, " %d %c", &value, &extra) == 1 &&
            value >= min && value <= max) {
            return value;
        }

        printf("Invalid input. Enter a number from %d to %d.\n",
               min, max);
    }
}

static void show_banner(void)
{
    printf("\n");
    printf("========================================\n");
    printf("       PROCESS SCHEDULING SIMULATOR\n");
    printf("========================================\n");
}

static void show_process_header(int number)
{
    printf("\n----------------------------------------\n");
    printf("             PROCESS %d\n", number);
    printf("----------------------------------------\n");
}

static void input_processes(Process p[], int n)
{
    for (int i = 0; i < n; i++) {
        char prompt[100];

        show_process_header(i + 1);

        snprintf(prompt, sizeof(prompt), "  Enter PID          : ");
        p[i].pid = read_int(prompt, 1, 999999);

        snprintf(prompt, sizeof(prompt), "  Enter Arrival Time : ");
        p[i].arrival_time = read_int(prompt, 0, 1000000);

        snprintf(prompt, sizeof(prompt), "  Enter Burst Time   : ");
        p[i].burst_time = read_int(prompt, 1, 1000000);

        snprintf(prompt, sizeof(prompt), "  Enter Priority     : ");
        p[i].priority = read_int(prompt, 0, 1000000);

        p[i].completion_time = 0;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].response_time = -1;
        p[i].remaining_time = p[i].burst_time;
    }

    /* Process IDs must be unique. */
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].pid == p[j].pid) {
                printf("\nError: PID %d is duplicated.\n", p[i].pid);
                printf("Please enter the process details again.\n");
                input_processes(p, n);
                return;
            }
        }
    }
}

static void show_process_table(const Process p[], int n)
{
    printf("\n");
    printf("============================================================\n");
    printf("                  PROCESS INPUT SUMMARY\n");
    printf("============================================================\n");
    printf("%-10s %-15s %-12s %-10s\n",
           "PID", "Arrival Time", "Burst Time", "Priority");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%-10d %-15d %-12d %-10d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].priority);
    }

    printf("============================================================\n");
    printf("Note: A smaller priority number means higher priority.\n");
}

static int show_algorithm_menu(void)
{
    printf("\n========================================\n");
    printf("          SCHEDULING ALGORITHMS\n");
    printf("========================================\n");
    printf("  1. First Come First Served (FCFS)\n");
    printf("  2. SJF Non-preemptive\n");
    printf("  3. SJF Preemptive\n");
    printf("  4. Priority Non-preemptive\n");
    printf("  5. Priority Preemptive\n");
    printf("  6. Round Robin\n");
    printf("  0. Exit\n");
    printf("----------------------------------------\n");

    return read_int("Choose an algorithm: ", 0, 6);
}

static void run_scheduler(Process original[], int n)
{
    Process p[MAX_PROCESSES];
    int choice;

    memcpy(p, original, (size_t)n * sizeof(Process));

    choice = show_algorithm_menu();

    if (choice == 0) {
        return;
    }

    reset_gantt();

    switch (choice) {
        case 1:
            printf("\nSelected: FCFS\n");
            fcfs(p, n);
            break;

        case 2:
            printf("\nSelected: SJF Non-preemptive\n");
            sjf_non_preemptive(p, n);
            break;

        case 3:
            printf("\nSelected: SJF Preemptive\n");
            sjf_preemptive(p, n);
            break;

        case 4:
            printf("\nSelected: Priority Non-preemptive\n");
            priority_non_preemptive(p, n);
            break;

        case 5:
            printf("\nSelected: Priority Preemptive\n");
            priority_preemptive(p, n);
            break;

        case 6: {
            int quantum = read_int(
                "Enter Round Robin time quantum: ", 1, 1000000);
            printf("\nSelected: Round Robin (Quantum = %d)\n",
                   quantum);
            round_robin(p, n, quantum);
            break;
        }

        default:
            printf("Invalid selection.\n");
            return;
    }

    printf("\n");
    print_gantt();
    calculate_metrics(p, n);
    print_results(p, n);
}

int main(void)
{
    Process processes[MAX_PROCESSES];
    int n;
    int choice;

    while (1) {
        show_banner();
        printf("  1. Enter processes and run scheduler\n");
        printf("  0. Exit\n");
        printf("----------------------------------------\n");

        choice = read_int("Choose an option: ", 0, 1);

        if (choice == 0) {
            printf("\nThank you for using the simulator!\n");
            break;
        }

        printf("\n========================================\n");
        printf("       PROCESS SCHEDULING INPUT\n");
        printf("========================================\n");

        n = read_int("Enter number of processes (1-20): ",
                     1, MAX_PROCESSES);

        input_processes(processes, n);
        show_process_table(processes, n);
        run_scheduler(processes, n);
    }

    return 0;
}

