#include "process.h"

/*
 * Priority Scheduling
 * Rule: smaller priority number means higher priority.
 * Tie-break: smaller arrival time, then smaller PID.
 *
 * This file does not contain main(), input handling, or result printing.
 */

static int better_priority(const Process *a, const Process *b) {
    if (a->priority != b->priority)
        return a->priority < b->priority;
    if (a->arrival_time != b->arrival_time)
        return a->arrival_time < b->arrival_time;
    return a->pid < b->pid;
}

static int choose_best(Process p[], int n, int time, int require_remaining) {
    int best = -1;

    for (int i = 0; i < n; ++i) {
        if (p[i].arrival_time <= time &&
            (!require_remaining || p[i].remaining_time > 0)) {
            if (best == -1 || better_priority(&p[i], &p[best]))
                best = i;
        }
    }
    return best;
}

void priority_non_preemptive(Process p[], int n) {
    int completed = 0;
    int time = 0;

    while (completed < n) {
        int best = choose_best(p, n, time, 1);

        if (best == -1) {
            int next = -1;
            for (int i = 0; i < n; ++i) {
                if (p[i].remaining_time > 0 &&
                    (next == -1 || p[i].arrival_time < p[next].arrival_time ||
                     (p[i].arrival_time == p[next].arrival_time &&
                      p[i].pid < p[next].pid))) {
                    next = i;
                }
            }

            if (next == -1)
                break;

            add_gantt_entry(0, time, p[next].arrival_time);
            time = p[next].arrival_time;
            continue;
        }

        int start = time;
        time += p[best].remaining_time;
        p[best].remaining_time = 0;
        p[best].completion_time = time;
        add_gantt_entry(p[best].pid, start, time);
        completed++;
    }
}

void priority_preemptive(Process p[], int n) {
    int completed = 0;
    int time = 0;

    while (completed < n) {
        int best = choose_best(p, n, time, 1);

        if (best == -1) {
            int next_arrival = -1;
            for (int i = 0; i < n; ++i) {
                if (p[i].remaining_time > 0 &&
                    (next_arrival == -1 ||
                     p[i].arrival_time < next_arrival)) {
                    next_arrival = p[i].arrival_time;
                }
            }

            if (next_arrival == -1)
                break;

            add_gantt_entry(0, time, next_arrival);
            time = next_arrival;
            continue;
        }

        /* Run one time unit so a newly arrived higher-priority process
           can preempt the current process at the next decision point. */
        add_gantt_entry(p[best].pid, time, time + 1);
        p[best].remaining_time--;
        time++;

        if (p[best].remaining_time == 0) {
            p[best].completion_time = time;
            completed++;
        }
    }
}
