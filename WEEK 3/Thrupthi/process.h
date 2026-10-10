#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES 20
#define MAX_GANTT 500

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
    int remaining_time;
} Process;

typedef struct {
    int pid;
    int start;
    int end;
} GanttEntry;

extern GanttEntry gantt[MAX_GANTT];
extern int gantt_count;

void fcfs(Process p[], int n);
void sjf_non_preemptive(Process p[], int n);
void sjf_preemptive(Process p[], int n);
void priority_non_preemptive(Process p[], int n);
void priority_preemptive(Process p[], int n);
void calculate_metrics(Process p[], int n);
void print_results(Process p[], int n);
void round_robin(Process p[], int n, int time_quantum);
void reset_gantt(void);
void add_gantt_entry(int pid, int start, int end);
void print_gantt(void);

#endif
