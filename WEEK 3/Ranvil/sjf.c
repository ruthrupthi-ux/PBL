#include "process.h"

static int tie_break(Process p[], int a, int b)
{
    if (p[a].arrival_time != p[b].arrival_time)
        return p[a].arrival_time < p[b].arrival_time;

    return p[a].pid < p[b].pid;
}

void sjf_non_preemptive(Process p[], int n)
{
    int completed[MAX_PROCESSES] = {0};
    int completed_count = 0;
    int current_time = 0;
    int i;

    if (p == 0 || n <= 0 || n > MAX_PROCESSES)
        return;

    for (i = 0; i < n; i++) {
        p[i].completion_time = 0;
        p[i].remaining_time = p[i].burst_time;
    }

    while (completed_count < n) {
        int selected = -1;

        for (i = 0; i < n; i++) {
            if (completed[i] ||
                p[i].arrival_time > current_time)
                continue;

            if (selected == -1 ||
                p[i].burst_time < p[selected].burst_time ||
                (p[i].burst_time == p[selected].burst_time &&
                 tie_break(p, i, selected))) {
                selected = i;
            }
        }

        if (selected == -1) {
            int next = -1;

            for (i = 0; i < n; i++) {
                if (completed[i])
                    continue;

                if (next == -1 ||
                    p[i].arrival_time < p[next].arrival_time ||
                    (p[i].arrival_time == p[next].arrival_time &&
                     p[i].pid < p[next].pid)) {
                    next = i;
                }
            }

            if (next == -1)
                break;

            if (current_time < p[next].arrival_time) {
                add_gantt_entry(0, current_time,
                                p[next].arrival_time);
                current_time = p[next].arrival_time;
            }

            continue;
        }

        {
            int start_time = current_time;
            int end_time = current_time + p[selected].burst_time;

            add_gantt_entry(p[selected].pid,
                            start_time, end_time);

            current_time = end_time;
            p[selected].completion_time = current_time;
            p[selected].remaining_time = 0;

            completed[selected] = 1;
            completed_count++;
        }
    }
}

void sjf_preemptive(Process p[], int n)
{
    int completed_count = 0;
    int current_time = 0;
    int i;

    if (p == 0 || n <= 0 || n > MAX_PROCESSES)
        return;

    for (i = 0; i < n; i++) {
        p[i].completion_time = 0;
        p[i].remaining_time = p[i].burst_time;
    }

    while (completed_count < n) {
        int selected = -1;

        for (i = 0; i < n; i++) {
            if (p[i].remaining_time <= 0 ||
                p[i].arrival_time > current_time)
                continue;

            if (selected == -1 ||
                p[i].remaining_time <
                    p[selected].remaining_time ||
                (p[i].remaining_time ==
                     p[selected].remaining_time &&
                 tie_break(p, i, selected))) {
                selected = i;
            }
        }

        if (selected == -1) {
            int next = -1;

            for (i = 0; i < n; i++) {
                if (p[i].remaining_time <= 0)
                    continue;

                if (next == -1 ||
                    p[i].arrival_time < p[next].arrival_time ||
                    (p[i].arrival_time == p[next].arrival_time &&
                     p[i].pid < p[next].pid)) {
                    next = i;
                }
            }

            if (next == -1)
                break;

            if (current_time < p[next].arrival_time) {
                add_gantt_entry(0, current_time,
                                p[next].arrival_time);
                current_time = p[next].arrival_time;
            }

            continue;
        }

        add_gantt_entry(p[selected].pid,
                        current_time, current_time + 1);

        p[selected].remaining_time--;
        current_time++;

        if (p[selected].remaining_time == 0) {
            p[selected].completion_time = current_time;
            completed_count++;
        }
    }
}