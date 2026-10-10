#include <stdio.h>
#include "process.h"

int main(void)
{
    Process p[MAX_PROCESSES];
    int n, quantum, i;

    printf("ROUND ROBIN CPU SCHEDULING\n");
    printf("Enter number of processes (1-%d): ", MAX_PROCESSES);

    if (scanf("%d", &n) != 1 ||
        n < 1 || n > MAX_PROCESSES) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        p[i].pid = i + 1;

        printf("\nProcess P%d\n", p[i].pid);

        printf("Arrival time: ");
        if (scanf("%d", &p[i].arrival_time) != 1 ||
            p[i].arrival_time < 0) {
            printf("Invalid arrival time.\n");
            return 1;
        }

        printf("Burst time: ");
        if (scanf("%d", &p[i].burst_time) != 1 ||
            p[i].burst_time <= 0) {
            printf("Invalid burst time.\n");
            return 1;
        }
    }

    printf("\nEnter time quantum: ");
    if (scanf("%d", &quantum) != 1 || quantum <= 0) {
        printf("Invalid time quantum.\n");
        return 1;
    }

    round_robin(p, n, quantum);
    print_gantt();

    printf("\nProcess  AT  BT  CT  TAT  WT  RT\n");

    for (i = 0; i < n; i++) {
        printf("P%-7d %-3d %-3d %-3d %-4d %-3d %-3d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].completion_time,
               p[i].turnaround_time,
               p[i].waiting_time,
               p[i].response_time);
    }

    return 0;
}
