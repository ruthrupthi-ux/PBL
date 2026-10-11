#include <stdio.h>
#include "process.h"

int main(void)
{
    Process p[MAX_PROCESSES];
    int n, quantum;

    printf("\n");
    printf("===============================================\n");
    printf("       ROUND ROBIN CPU SCHEDULING TEST\n");
    printf("===============================================\n");

    printf("\nEnter number of processes (1-%d): ",
           MAX_PROCESSES);

    if (scanf("%d", &n) != 1 ||
        n < 1 || n > MAX_PROCESSES) {
        printf("Invalid number of processes!\n");
        return 1;
    }

    printf("\nEnter process details:\n");
    printf("------------------------------------------------\n");
    printf("%-10s %-15s %-15s\n",
           "PID", "Arrival Time", "Burst Time");
    printf("------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("\nProcess P%d\n", i + 1);

        printf("  Enter PID: ");
        if (scanf("%d", &p[i].pid) != 1 ||
            p[i].pid == 0) {
            printf("Invalid PID!\n");
            return 1;
        }

        for (int j = 0; j < i; j++) {
            if (p[j].pid == p[i].pid) {
                printf("Duplicate PID! Use unique PIDs.\n");
                return 1;
            }
        }

        printf("  Enter Arrival Time: ");
        if (scanf("%d", &p[i].arrival_time) != 1 ||
            p[i].arrival_time < 0) {
            printf("Arrival time must be >= 0.\n");
            return 1;
        }

        printf("  Enter Burst Time: ");
        if (scanf("%d", &p[i].burst_time) != 1 ||
            p[i].burst_time <= 0) {
            printf("Burst time must be greater than 0.\n");
            return 1;
        }

        p[i].priority = 0;
    }

    printf("\n------------------------------------------------\n");
    printf("%-10s %-15s %-15s\n",
           "PID", "Arrival Time", "Burst Time");
    printf("------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%-10d %-15d %-15d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time);
    }

    printf("------------------------------------------------\n");

    printf("\nEnter Time Quantum: ");
    if (scanf("%d", &quantum) != 1 || quantum <= 0) {
        printf("Time quantum must be greater than 0.\n");
        return 1;
    }

    round_robin(p, n, quantum);

    print_gantt();

    double total_wt = 0.0;
    double total_tat = 0.0;
    double total_rt = 0.0;

    printf("\n");
    printf("===============================================================\n");
    printf("                    SCHEDULING RESULTS\n");
    printf("===============================================================\n");

    printf("%-6s %-5s %-5s %-5s %-6s %-5s %-5s\n",
           "PID", "AT", "BT", "CT", "TAT", "WT", "RT");

    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        int tat = p[i].completion_time - p[i].arrival_time;
        int wt = tat - p[i].burst_time;
        int rt = p[i].response_time;

        p[i].turnaround_time = tat;
        p[i].waiting_time = wt;

        total_wt += wt;
        total_tat += tat;
        total_rt += rt;

        printf("%-6d %-5d %-5d %-5d %-6d %-5d %-5d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].completion_time,
               tat,
               wt,
               rt);
    }

    printf("---------------------------------------------------------------\n");

    printf("\nAverage Waiting Time    : %.2f\n",
           total_wt / n);

    printf("Average Turnaround Time : %.2f\n",
           total_tat / n);

    printf("Average Response Time   : %.2f\n",
           total_rt / n);

    printf("\nTest completed.\n");

    return 0;
}
