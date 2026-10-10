
#include <stdio.h>
#include "process.h"

GanttEntry gantt[MAX_GANTT];
int gantt_count = 0;

void reset_gantt(void)
{
    gantt_count = 0;
}

void add_gantt_entry(int pid, int start, int end)
{
    if (gantt_count < MAX_GANTT) {
        gantt[gantt_count].pid = pid;
        gantt[gantt_count].start = start;
        gantt[gantt_count].end = end;
        gantt_count++;
    }
}

void print_gantt(void)
{
    int i;

    printf("\nGantt Chart:\n");

    if (gantt_count == 0) {
        printf("No execution recorded.\n");
        return;
    }

    printf("%d", gantt[0].start);

    for (i = 0; i < gantt_count; i++) {
        if (gantt[i].pid == 0)
            printf(" | IDLE | %d", gantt[i].end);
        else
            printf(" | P%d | %d", gantt[i].pid, gantt[i].end);
    }

    printf("\n");
}

typedef struct {
    int data[MAX_PROCESSES];
    int front, rear, count;
} ReadyQueue;

static void enqueue(ReadyQueue *q, int process)
{
    q->data[q->rear] = process;
    q->rear = (q->rear + 1) % MAX_PROCESSES;
    q->count++;
}

static int dequeue(ReadyQueue *q)
{
    int process = q->data[q->front];
    q->front = (q->front + 1) % MAX_PROCESSES;
    q->count--;
    return process;
}

static void add_arrivals(Process p[], int n, int time,
                         int arrived[], ReadyQueue *q)
{
    int i;

    while (1) {
        int next = -1;

        for (i = 0; i < n; i++) {
            if (!arrived[i] && p[i].arrival_time <= time) {
                if (next == -1 ||
                    p[i].arrival_time < p[next].arrival_time) {
                    next = i;
                }
            }
        }

        if (next == -1)
            break;

        enqueue(q, next);
        arrived[next] = 1;
    }
}

void round_robin(Process p[], int n, int time_quantum)
{
    ReadyQueue q = {{0}, 0, 0, 0};
    int arrived[MAX_PROCESSES] = {0};
    int time, completed = 0;
    int i;

    if (n < 1 || n > MAX_PROCESSES || time_quantum <= 0) {
        printf("Invalid process count or time quantum.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        if (p[i].arrival_time < 0 || p[i].burst_time <= 0) {
            printf("Invalid arrival time or burst time.\n");
            return;
        }
    }

    reset_gantt();

    for (i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        p[i].completion_time = 0;
        p[i].turnaround_time = 0;
        p[i].waiting_time = 0;
        p[i].response_time = -1;
    }

    time = p[0].arrival_time;

    for (i = 1; i < n; i++) {
        if (p[i].arrival_time < time)
            time = p[i].arrival_time;
    }

    while (completed < n) {
        int current, start, run;

        add_arrivals(p, n, time, arrived, &q);

        if (q.count == 0) {
            int next_time = -1;

            for (i = 0; i < n; i++) {
                if (!arrived[i] &&
                    (next_time == -1 ||
                     p[i].arrival_time < next_time)) {
                    next_time = p[i].arrival_time;
                }
            }

            if (next_time == -1)
                break;

            if (next_time > time) {
                add_gantt_entry(0, time, next_time);
                time = next_time;
            }

            continue;
        }

        current = dequeue(&q);
        start = time;

        if (p[current].response_time == -1) {
            p[current].response_time =
                time - p[current].arrival_time;
        }

        run = p[current].remaining_time;

        if (run > time_quantum)
            run = time_quantum;

        time += run;
        p[current].remaining_time -= run;

        add_gantt_entry(p[current].pid, start, time);

        add_arrivals(p, n, time, arrived, &q);

        if (p[current].remaining_time > 0) {
            enqueue(&q, current);
        } else {
            p[current].completion_time = time;
            completed++;
        }
    }

    for (i = 0; i < n; i++) {
        p[i].turnaround_time =
            p[i].completion_time - p[i].arrival_time;

        p[i].waiting_time =
            p[i].turnaround_time - p[i].burst_time;
    }
}

