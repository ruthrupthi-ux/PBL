
#include <stdio.h>
#include "process.h"

static void reset_processes(Process dest[],
                            const Process source[], int n)
{
    for (int i = 0; i < n; i++) {
        dest[i] = source[i];
        dest[i].completion_time = 0;
        dest[i].remaining_time = dest[i].burst_time;
    }
}

static void print_completion_times(Process p[], int n)
{
    printf("\nPID\tAT\tBT\tCT\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].completion_time);
    }
}

int main(void)
{
    Process original[] = {
        {1, 0, 5, 2, 0, 0, 0, 0, 5},
        {2, 1, 3, 1, 0, 0, 0, 0, 3},
        {3, 2, 4, 3, 0, 0, 0, 0, 4},
        {4, 3, 2, 4, 0, 0, 0, 0, 2}
    };

    Process p[ MAX_PROCESSES ];
    int n = sizeof(original) / sizeof(original[0]);

    /* Test SJF Non-Preemptive. */
    reset_processes(p, original, n);

    printf("SJF Non-Preemptive Test\n");
    reset_gantt();
    sjf_non_preemptive(p, n);
    print_gantt();
    print_completion_times(p, n);

    /* Test SJF Preemptive (SRTF). */
    reset_processes(p, original, n);

    printf("\nSJF Preemptive (SRTF) Test\n");
    reset_gantt();
    sjf_preemptive(p, n);
    print_gantt();
    print_completion_times(p, n);

    return 0;
}
