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
    if (start >= end)
        return;

    if (gantt_count > 0 &&
        gantt[gantt_count - 1].pid == pid &&
        gantt[gantt_count - 1].end == start)
    {
        gantt[gantt_count - 1].end = end;
        return;
    }

    if (gantt_count >= MAX_GANTT)
    {
        printf("Warning: Gantt chart capacity reached.\n");
        return;
    }

    gantt[gantt_count].pid = pid;
    gantt[gantt_count].start = start;
    gantt[gantt_count].end = end;

    gantt_count++;
}

void print_gantt(void)
{
    if (gantt_count == 0)
    {
        printf("\nNo Gantt chart data available.\n");
        return;
    }

    printf("\nGantt Chart:\n");

    for (int i = 0; i < gantt_count; i++)
    {
        char label[16];

        if (gantt[i].pid == 0)
            snprintf(label, sizeof(label), "IDLE");
        else
            snprintf(label, sizeof(label), "P%d", gantt[i].pid);

        printf("| %-6s", label);
    }

    printf("|\n");

    printf("%-8d", gantt[0].start);

    for (int i = 0; i < gantt_count; i++)
    {
        printf("%-8d", gantt[i].end);
    }

    printf("\n");
}
