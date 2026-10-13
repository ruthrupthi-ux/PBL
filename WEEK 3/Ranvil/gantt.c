
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
    if (end <= start)
        return;

    if (gantt_count > 0) {
        GanttEntry *last = &gantt[gantt_count - 1];

        if (last->pid == pid && last->end == start) {
            last->end = end;
            return;
        }
    }

    if (gantt_count >= MAX_GANTT) {
        fprintf(stderr, "Error: Gantt chart capacity exceeded.\n");
        return;
    }

    gantt[gantt_count].pid = pid;
    gantt[gantt_count].start = start;
    gantt[gantt_count].end = end;

    gantt_count++;
}

void print_gantt(void)
{
    int i;

    if (gantt_count == 0) {
        printf("\nGantt chart is empty.\n");
        return;
    }

    printf("\nGantt Chart:\n");

    for (i = 0; i < gantt_count; i++) {
        char label[20];

        if (gantt[i].pid == 0)
            snprintf(label, sizeof(label), "IDLE");
        else
            snprintf(label, sizeof(label), "P%d", gantt[i].pid);

        printf("| %-7s", label);
    }

    printf("|\n");

    printf("%-9d", gantt[0].start);

    for (i = 0; i < gantt_count; i++) {
        printf("%-9d", gantt[i].end);
    }

    printf("\n");
}
