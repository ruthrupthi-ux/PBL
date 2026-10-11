#include <stdio.h>
#include "process.h"

typedef struct {
    int data[MAX_PROCESSES];
    int front;
    int rear;
    int count;
} ReadyQueue;

static void enqueue(ReadyQueue *q, int index)
{
    if (q->count >= MAX_PROCESSES)
        return;

    q->data[q->rear] = index;
    q->rear = (q->rear + 1) % MAX_PROCESSES;
    q->count++;
}

static int dequeue(ReadyQueue *q)
{
    int index = q->data[q->front];
    q->front = (q->front + 1) % MAX_PROCESSES;
    q->count--;
    return index;
}

static void add_arrivals(Process p[], int n, int time,
                         int arrived[], ReadyQueue *q)
{
    while (1) {
        int next = -1;

        for (int i = 0; i < n; i++) {
            if (!arrived[i] && p[i].arrival_time <= time) {
                if (next == -1 ||
                    p[i].arrival_time < p[next].arrival_time ||
                    (p[i].arrival_time == p[next].arrival_time &&
                     p[i].pid < p[next].pid)) {
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
    int time = 0;
    int completed = 0;

    if (p == NULL || n < 1 || n > MAX_PROCESSES ||
        time_quantum <= 0) {
        printf("Invalid process count or time quantum.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        if (p[i].arrival_time < 0 || p[i].burst_time <= 0) {
            printf("Arrival time must be >= 0 and burst time > 0.\n");
            return;
        }
    }

    reset_gantt();

    for (int i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        p[i].completion_time = 0;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].response_time = -1;
    }

    time = 0;

    while (completed < n) {
        add_arrivals(p, n, time, arrived, &q);

        if (q.count == 0) {
            int next_time = -1;

            for (int i = 0; i < n; i++) {
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

        int current = dequeue(&q);
        int start = time;
        int run = p[current].remaining_time;

        if (run > time_quantum)
            run = time_quantum;

        if (p[current].response_time == -1)
            p[current].response_time =
                start - p[current].arrival_time;

        time += run;
        p[current].remaining_time -= run;

        add_gantt_entry(p[current].pid, start, time);

  
        add_arrivals(p, n, time, arrived, &q);

        if (p[current].remaining_time == 0) {
            p[current].completion_time = time;
            completed++;
        } else {
            enqueue(&q, current);
        }
    }

  
    for (int i = 0; i < n; i++) {
        p[i].turnaround_time =
            p[i].completion_time - p[i].arrival_time;

        p[i].waiting_time =
            p[i].turnaround_time - p[i].burst_time;
    }
}
