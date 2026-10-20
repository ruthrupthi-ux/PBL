#ifndef DEADLOCK_H
#define DEADLOCK_H

#include "banker.h"

int detectDeadlock(
    int processes,
    int resources,
    int available[MAX_RESOURCES],
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int request[MAX_PROCESSES][MAX_RESOURCES],
    int deadlocked[MAX_PROCESSES]
);

void printDeadlockedProcesses(
    int processes,
    int deadlocked[MAX_PROCESSES]
);

#endif
