
#include <stdio.h>
#include "process.h"

static void run_test(const char *name,
                     void (*algorithm)(Process[], int),
                     const Process original[],
                     int n)
{
    Process working[MAX_PROCESSES];
    int i;

    for (i = 0; i < n; i++) {
        working[i] = original[i];
        working[i].completion_time = 0;
        working[i].waiting_time = 0;
        working[i].turnaround_time = 0;
        working[i].response_time = 0;
        working[i].remaining_time = working[i].burst_time;
    }

    printf("\n========================================\n");
    printf("%s\n", name);
    printf("========================================\n");

    reset_gantt();
    algorithm(working, n);

    print_gantt();

    printf("\nCompletion Times:\n");
    printf("PID\tAT\tBT\tCT\n");

    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\n",
               working[i].pid,
               working[i].arrival_time,
               working[i].burst_time,
               working[i].completion_time);
    }
}

int main(void)
{
    Process test1[] = {
        {1, 0, 5, 2, 0, 0, 0, 0, 5},
        {2, 1, 3, 1, 0, 0, 0, 0, 3},
        {3, 2, 4, 3, 0, 0, 0, 0, 4},
        {4, 3, 2, 4, 0, 0, 0, 0, 2}
    };

    int n1 = (int)(sizeof(test1) / sizeof(test1[0]));

    run_test("FCFS", fcfs, test1, n1);

    run_test("SJF Non-Preemptive",
             sjf_non_preemptive, test1, n1);

    run_test("SJF Preemptive (SRTF)",
             sjf_preemptive, test1, n1);

    {
        Process test2[] = {
            {1, 3, 4, 2, 0, 0, 0, 0, 4},
            {2, 4, 2, 1, 0, 0, 0, 0, 2},
            {3, 8, 1, 3, 0, 0, 0, 0, 1}
        };

        int n2 = (int)(sizeof(test2) / sizeof(test2[0]));

        run_test("FCFS - Idle Time Test", fcfs, test2, n2);

        run_test("SJF Non-Preemptive - Idle Time Test",
                 sjf_non_preemptive, test2, n2);

        run_test("SJF Preemptive - Idle Time Test",
                 sjf_preemptive, test2, n2);
    }

    return 0;
}
