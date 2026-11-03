#include <stdio.h>
#include "process.h"

int main(void)
{
    Process p[] = {
        {1, 0, 5, 2, 0, 0, 0, 0, 5},
        {2, 1, 3, 1, 0, 0, 0, 0, 3},
        {3, 2, 4, 3, 0, 0, 0, 0, 4},
        {4, 3, 2, 4, 0, 0, 0, 0, 2}
    };

    int n = sizeof(p) / sizeof(p[0]);

    printf("FCFS Scheduling Test\n");

    reset_gantt();
    fcfs(p, n);
    print_gantt();

    printf("\nPID\tAT\tBT\tCT\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].completion_time);
    }

    return 0;
}
