#include <stdio.h>
#include "recovery.h"

int recoverByProcessTermination(
    int process,
    int processes,
    int resources,
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int available[MAX_RESOURCES],
    int deadlocked[MAX_PROCESSES]
) {
    if (process < 0 || process >= processes) {
        printf("Invalid process number.\n");
        return 0;
    }

    if (!deadlocked[process]) {
        printf("Process P%d is not marked as deadlocked.\n", process);
        return 0;
    }

    printf("\nRecovering from deadlock...\n");
    printf("Terminating Process P%d\n", process);

    for (int j = 0; j < resources; j++) {

        available[j] += allocation[process][j];

        allocation[process][j] = 0;
    }

    deadlocked[process] = 0;

    printf("Resources held by P%d have been released.\n", process);

    printAvailableResources(resources, available);

    return 1;
}

void printAvailableResources(
    int resources,
    int available[MAX_RESOURCES]
) {
    printf("\nAvailable Resources:\n");

    for (int j = 0; j < resources; j++) {
        printf("R%d = %d\n", j, available[j]);
    }

    printf("\n");
}
