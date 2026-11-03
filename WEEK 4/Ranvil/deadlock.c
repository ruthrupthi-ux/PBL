#include <stdio.h>
#include "deadlock.h"

int detectDeadlock(
    int processes,
    int resources,
    int available[MAX_RESOURCES],
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int request[MAX_PROCESSES][MAX_RESOURCES],
    int deadlocked[MAX_PROCESSES]
) {
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES];

    for (int j = 0; j < resources; j++) {
        work[j] = available[j];
    }

    for (int i = 0; i < processes; i++) {

        int hasAllocation = 0;

        for (int j = 0; j < resources; j++) {
            if (allocation[i][j] > 0) {
                hasAllocation = 1;
                break;
            }
        }

        if (hasAllocation) {
            finish[i] = 0;
        } else {
            finish[i] = 1;
        }

        deadlocked[i] = 0;
    }

    int changed = 1;

    while (changed) {

        changed = 0;

        for (int i = 0; i < processes; i++) {

            if (finish[i] == 0) {

                int canProceed = 1;

                for (int j = 0; j < resources; j++) {

                    if (request[i][j] > work[j]) {
                        canProceed = 0;
                        break;
                    }
                }

                if (canProceed) {

                    for (int j = 0; j < resources; j++) {
                        work[j] += allocation[i][j];
                    }

                    finish[i] = 1;
                    changed = 1;
                }
            }
        }
    }

    int deadlockCount = 0;

    for (int i = 0; i < processes; i++) {

        if (finish[i] == 0) {
            deadlocked[i] = 1;
            deadlockCount++;
        }
    }

    return deadlockCount;
}

void printDeadlockedProcesses(
    int processes,
    int deadlocked[MAX_PROCESSES]
) {
    printf("\n========== DEADLOCK DETECTION ==========\n");

    int found = 0;

    for (int i = 0; i < processes; i++) {

        if (deadlocked[i]) {
            printf("Process P%d is DEADLOCKED\n", i);
            found = 1;
        }
    }

    if (!found) {
        printf("No deadlocked processes detected.\n");
    }

    printf("========================================\n");
}
