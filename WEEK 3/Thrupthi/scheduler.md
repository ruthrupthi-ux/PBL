
# CPU Process Scheduling Simulator

## 1. Project Overview

The CPU Process Scheduling Simulator is a C-based application that implements different CPU scheduling algorithms. It allows users to enter process details, select a scheduling algorithm, and analyze process execution using a Gantt chart and performance metrics.

## 2. Objectives

- Implement various CPU scheduling algorithms.
- Simulate process execution using arrival time, burst time, and priority.
- Calculate and analyze scheduling performance metrics.
- Represent process execution using a Gantt chart.
- Understand preemptive and non-preemptive scheduling.

## 3. Scheduling Algorithms

- First-Come, First-Served (FCFS)
- Shortest Job First (SJF) - Non-preemptive
- Shortest Job First (SJF) - Preemptive
- Priority Scheduling - Non-preemptive
- Priority Scheduling - Preemptive
- Round Robin (RR) - Configurable time quantum

## 4. Project Structure

```text
WEEK 3/
├── Ranvil/
│   ├── fcfs.c
│   ├── sjf.c
│   └── Test files
├── Thrupthi/
│   ├── main.c
│   ├── process.h
│   └── README.md
├── Ananya/
│   ├── priority.c
│   ├── metrics.c
│   └── Test files
└── Sourabh/
    ├── gantt.c
    ├── round_robin.c
    └── test_rr.c
```

## 5. Features

- Menu-driven command-line interface
- Process input using Process ID, Arrival Time, Burst Time, and Priority
- Multiple CPU scheduling algorithms
- Gantt chart representation
- Calculation of scheduling performance metrics
- Configurable time quantum for Round Robin

## 6. Performance Metrics

| Metric | Description |
|---|---|
| Completion Time (CT) | Time at which a process completes execution |
| Turnaround Time (TAT) | Completion Time - Arrival Time |
| Waiting Time (WT) | Turnaround Time - Burst Time |
| Response Time (RT) | First CPU Start Time - Arrival Time |
| Average Waiting Time | Average waiting time of all processes |
| Average Turnaround Time | Average turnaround time of all processes |
| CPU Utilization | Percentage of elapsed simulation time during which the CPU is executing processes |



## 7. Compilation and Execution

Run the following commands from the repository root directory.

### Step 1: Navigate to the Repository

```bash
cd ~/PBL
```

### Step 2: Compile the Source Files

```bash
gcc -Wall -Wextra -std=c11 \
-I"WEEK 3/Thrupthi" \
"WEEK 3/Thrupthi/main.c" \
"WEEK 3/Ananya/metrics.c" \
"WEEK 3/Ananya/priority.c" \
"WEEK 3/Ranvil/fcfs.c" \
"WEEK 3/Ranvil/sjf.c" \
"WEEK 3/Sourabh/gantt.c" \
"WEEK 3/Sourabh/round_robin.c" \
-o "WEEK 3/Thrupthi/scheduler"
```

### Step 3: Run the Simulator

```bash
./"WEEK 3/Thrupthi/scheduler"
```

### Step 4: Execute a Simulation

1. Enter the number of processes.
2. Enter the Process ID, Arrival Time, Burst Time, and Priority for each process.
3. Select the required scheduling algorithm.
4. Enter the time quantum when using Round Robin.
5. View the Gantt chart and scheduling results.
6. Select `0` to exit.

## 8. Team Responsibilities

| Team Member | Responsibility |
|---|---|
| Thrupthi | Main program, process header, and integration |
| Ranvil | FCFS and SJF scheduling algorithms |
| Ananya | Priority scheduling and performance metrics |
| Sourabh | Round Robin scheduling and Gantt chart |

## 9. Conclusion

The CPU Process Scheduling Simulator demonstrates the implementation of CPU scheduling algorithms in C. It helps users understand how different scheduling techniques manage process execution and evaluate their performance using scheduling metrics.
