#ifndef BANKER_H
#define BANKER_H

#define MAX_PROCESSES 20
#define MAX_RESOURCES 10

void calculateNeed(
    int processes,
    int resources,
    int maximum[MAX_PROCESSES][MAX_RESOURCES],
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int need[MAX_PROCESSES][MAX_RESOURCES]
);

int isSafeState(
    int processes,
    int resources,
    int available[MAX_RESOURCES],
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int need[MAX_PROCESSES][MAX_RESOURCES],
    int safeSequence[MAX_PROCESSES]
);

void printNeedMatrix(
    int processes,
    int resources,
    int need[MAX_PROCESSES][MAX_RESOURCES]
);

#endif
