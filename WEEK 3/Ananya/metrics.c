#include "process.h"
#include <stdio.h>

void calculate_metrics(Process p[], int n) {
    for (int i = 0; i < n; ++i) {
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

        p[i].response_time = -1;
        for (int j = 0; j < gantt_count; ++j) {
            if (gantt[j].pid == p[i].pid) {
                p[i].response_time = gantt[j].start - p[i].arrival_time;
                break;
            }
        }

        /* A valid scheduled process should always appear in the Gantt chart. */
        if (p[i].response_time < 0)
            p[i].response_time = 0;
    }
}

void print_results(Process p[], int n) {
    double total_wt = 0.0;
    double total_tat = 0.0;
    double total_rt = 0.0;
    int last_end = 0;
    int busy_time = 0;

    printf("\n%-5s %-5s %-5s %-9s %-5s %-5s %-6s %-5s\n",
           "PID", "AT", "BT", "Priority", "CT", "WT", "TAT", "RT");
    printf("---------------------------------------------------------\n");

    for (int i = 0; i < n; ++i) {
        printf("%-5d %-5d %-5d %-9d %-5d %-5d %-6d %-5d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].priority, p[i].completion_time, p[i].waiting_time,
               p[i].turnaround_time, p[i].response_time);

        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
        total_rt += p[i].response_time;
    }

    for (int i = 0; i < gantt_count; ++i) {
        if (gantt[i].end > last_end)
            last_end = gantt[i].end;

        if (gantt[i].pid != 0)
            busy_time += gantt[i].end - gantt[i].start;
    }

    double cpu_utilization = 0.0;
    if (last_end > 0)
        cpu_utilization = ((double)busy_time / last_end) * 100.0;

    printf("\nAverage Waiting Time : %.2f\n", total_wt / n);
    printf("Average Turnaround Time : %.2f\n", total_tat / n);
    printf("Average Response Time : %.2f\n", total_rt / n);
    printf("CPU Utilization : %.2f%%\n", cpu_utilization);
}
