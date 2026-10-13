
#include <stdio.h>
#include "process.h"

void run_scheduler(void)
{
    Process p[MAX_PROCESSES];
    int n, choice, quantum = 0;

    printf("\n====================================\n");
    printf("       PROCESS SCHEDULING INPUT\n");
    printf("====================================\n");

    printf("Enter number of processes (1-%d): ", MAX_PROCESSES);

    if (scanf("%d", &n) != 1 ||
        n < 1 || n > MAX_PROCESSES) {
        printf("Invalid number of processes.\n");
        return;
    }

    /* Read process details */
    for (int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i + 1);
        printf("Enter PID, Arrival Time, Burst Time, Priority: ");

        if (scanf("%d %d %d %d",
                  &p[i].pid,
                  &p[i].arrival_time,
                  &p[i].burst_time,
                  &p[i].priority) != 4) {
            printf("Invalid input.\n");
            return;
        }

        if (p[i].pid <= 0 ||
            p[i].arrival_time < 0 ||
            p[i].burst_time <= 0) {
            printf("Invalid process details.\n");
            return;
        }

        /* Ensure PIDs are unique */
        for (int j = 0; j < i; j++) {
            if (p[j].pid == p[i].pid) {
                printf("Duplicate PID. Enter unique PIDs.\n");
                return;
            }
        }

        p[i].completion_time = 0;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].response_time = -1;
        p[i].remaining_time = p[i].burst_time;
    }

    /* Algorithm selection */
    printf("\n====================================\n");
    printf("       SELECT SCHEDULING ALGORITHM\n");
    printf("====================================\n");
    printf("1. FCFS\n");
    printf("2. SJF Non-preemptive\n");
    printf("3. SJF Preemptive\n");
    printf("4. Priority Non-preemptive\n");
    printf("5. Priority Preemptive\n");
    printf("6. Round Robin\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1 ||
        choice < 1 || choice > 6) {
        printf("Invalid algorithm choice.\n");
        return;
    }

    if (choice == 6) {
        printf("Enter time quantum: ");

        if (scanf("%d", &quantum) != 1 || quantum <= 0) {
            printf("Time quantum must be greater than zero.\n");
            return;
        }
    }

    /* Reset shared scheduling data */
    reset_gantt();

    for (int i = 0; i < n; i++) {
        p[i].completion_time = 0;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].response_time = -1;
        p[i].remaining_time = p[i].burst_time;
    }

    /* Run selected algorithm */
    printf("\nRunning selected algorithm...\n");

    switch (choice) {
        case 1:
            printf("\nAlgorithm: FCFS\n");
            fcfs(p, n);
            break;

        case 2:
            printf("\nAlgorithm: SJF Non-preemptive\n");
            sjf_non_preemptive(p, n);
            break;

        case 3:
            printf("\nAlgorithm: SJF Preemptive\n");
            sjf_preemptive(p, n);
            break;

        case 4:
            printf("\nAlgorithm: Priority Non-preemptive\n");
            priority_non_preemptive(p, n);
            break;

        case 5:
            printf("\nAlgorithm: Priority Preemptive\n");
            priority_preemptive(p, n);
            break;

        case 6:
            printf("\nAlgorithm: Round Robin\n");
            round_robin(p, n, quantum);
            break;
    }

    /* Display results */
    print_gantt();
    calculate_metrics(p, n);
    print_results(p, n);
}

int main(void)
{
    int choice;

    while (1) {
        printf("\n====================================\n");
        printf("     PROCESS SCHEDULING SIMULATOR\n");
        printf("====================================\n");
        printf("1. Run Scheduler\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            return 1;
        }

        switch (choice) {
            case 1:
                run_scheduler();
                break;

            case 0:
                printf("Exiting simulator. Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice. Select 1 or 0.\n");
        }
    }
}
