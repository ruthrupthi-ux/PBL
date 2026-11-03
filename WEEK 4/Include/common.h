#ifndef COMMON_H
#define COMMON_H

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

typedef enum {
    PROCESS_ACTIVE = 0,
    PROCESS_FINISHED,
    PROCESS_DEADLOCKED,
    PROCESS_ABORTED
} ProcessStatus;

typedef struct {
    int process_count;
    int resource_count;

    /* Total instances of each resource type */
    int total_resources[MAX_RESOURCES];

    /* Instances currently available */
    int available[MAX_RESOURCES];

    /* Instances currently allocated to each process */
    int allocation[MAX_PROCESSES][MAX_RESOURCES];

    /* Maximum declared demand of each process */
    int max[MAX_PROCESSES][MAX_RESOURCES];

    /* Remaining declared need: max - allocation */
    int need[MAX_PROCESSES][MAX_RESOURCES];

    /* Current outstanding request for each process */
    int request[MAX_PROCESSES][MAX_RESOURCES];

    /* Current state of each process */
    ProcessStatus status[MAX_PROCESSES];

} SystemState;

#endif
