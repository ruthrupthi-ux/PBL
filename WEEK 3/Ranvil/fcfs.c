#include "process.h"

static void sort_by_arrival(Process p[], int order[], int n)
{
    int i, j;

    for (i = 0; i < n; i++) {
        order[i] = i;
    }

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            int a = order[j];
            int b = order[j + 1];

            if (p[a].arrival_time > p[b].arrival_time ||
                (p[a].arrival_time == p[b].arrival_time &&
                 p[a].pid > p[b].pid)) {
                int temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }
}

void fcfs(Process p[], int n)
{
    int order[MAX_PROCESSES];
    int i;
    int current_time = 0;

    if (p == 0 || n <= 0 || n > MAX_PROCESSES) {
        return;
    }

    for (i = 0; i < n; i++) {
        p[i].completion_time = 0;
        p[i].remaining_time = p[i].burst_time;
    }

    sort_by_arrival(p, order, n);

    for (i = 0; i < n; i++) {
        int index = order[i];
        int start_time;
        int end_time;

        if (current_time < p[index].arrival_time) {
            add_gantt_entry(0, current_time,
                            p[index].arrival_time);
            current_time = p[index].arrival_time;
        }

        start_time = current_time;
        end_time = start_time + p[index].burst_time;

        add_gantt_entry(p[index].pid, start_time, end_time);

        current_time = end_time;
        p[index].completion_time = current_time;
        p[index].remaining_time = 0;
    }
}
