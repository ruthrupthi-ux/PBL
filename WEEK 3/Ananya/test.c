#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GanttEntry gantt[MAX_GANTT_ENTRIES];
int gantt_count = 0;

/* Store each scheduling interval; adjacent intervals are kept separate
   so the Gantt chart reflects each scheduling decision. */
void add_gantt_entry(int pid, int start, int end) {
    if (end <= start) return;
    if (gantt_count >= MAX_GANTT_ENTRIES) {
        fprintf(stderr, "Error: Gantt chart capacity exceeded.\n");
        exit(EXIT_FAILURE);
    }
    gantt[gantt_count].pid = pid;
    gantt[gantt_count].start = start;
    gantt[gantt_count].end = end;
    gantt_count++;
}

static void reset_gantt(void) {
    gantt_count = 0;
}

static void initialize_processes(Process p[], const Process input[], int n) {
    for (int i = 0; i < n; ++i) {
        p[i] = input[i];
        p[i].remaining_time = p[i].burst_time;
        p[i].completion_time = 0;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].response_time = 0;
    }
}

static void print_gantt(void) {
    printf("\nGantt Chart:\n");
    for (int i = 0; i < gantt_count; ++i) {
        if (gantt[i].pid == 0)
            printf("| IDLE (%d-%d) ", gantt[i].start, gantt[i].end);
        else
            printf("| P%d (%d-%d) ", gantt[i].pid,
                   gantt[i].start, gantt[i].end);
    }
    printf("|\n");
}

int main(int argc, char *argv[]) {
    /* Smaller priority number means higher priority, matching priorityyyy.c. */
    const Process sample[] = {
        {1, 0, 7, 3, 7, 0, 0, 0, 0},
        {2, 2, 4, 1, 4, 0, 0, 0, 0},
        {3, 4, 1, 2, 1, 0, 0, 0, 0},
        {4, 6, 5, 1, 5, 0, 0, 0, 0},
        {5, 10, 2, 4, 2, 0, 0, 0, 0}
    };
    const int n = (int)(sizeof(sample) / sizeof(sample[0]));
    Process p[sizeof(sample) / sizeof(sample[0])];

    if (argc > 2 ||
        (argc == 2 && strcmp(argv[1], "np") != 0 &&
         strcmp(argv[1], "p") != 0 && strcmp(argv[1], "both") != 0)) {
        fprintf(stderr, "Usage: %s [np|p|both]\n", argv[0]);
        fprintf(stderr, "  np   = non-preemptive priority scheduling\n");
        fprintf(stderr, "  p    = preemptive priority scheduling\n");
        fprintf(stderr, "  both = run both (default)\n");
        return EXIT_FAILURE;
    }

    const char *mode = (argc == 2) ? argv[1] : "both";

    if (strcmp(mode, "np") == 0 || strcmp(mode, "both") == 0) {
        initialize_processes(p, sample, n);
        reset_gantt();
        printf("=== NON-PREEMPTIVE PRIORITY SCHEDULING ===\n");
        priority_non_preemptive(p, n);
        calculate_metrics(p, n);
        print_gantt();
        print_results(p, n);
    }

    if (strcmp(mode, "p") == 0 || strcmp(mode, "both") == 0) {
        initialize_processes(p, sample, n);
        reset_gantt();
        printf("\n=== PREEMPTIVE PRIORITY SCHEDULING ===\n");
        priority_preemptive(p, n);
        calculate_metrics(p, n);
        print_gantt();
        print_results(p, n);
    }

    return EXIT_SUCCESS;
}
