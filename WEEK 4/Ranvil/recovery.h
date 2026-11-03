#ifndef RECOVERY_H
#define RECOVERY_H

#include "../Include/common.h"

int recoverByProcessTermination(
    int process,
    int processes,
    int resources,
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int available[MAX_RESOURCES],
    int deadlocked[MAX_PROCESSES]
);

void printAvailableResources(
    int resources,
    int available[MAX_RESOURCES]
);

#endif
