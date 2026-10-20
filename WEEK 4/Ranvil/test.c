#include <stdio.h>

#include "banker.h"
#include "deadlock.h"
#include "recovery.h"

#define PROCESSES 5
#define RESOURCES 3

int main() {

    int available[MAX_RESOURCES] = {
        3, 3, 2
    };

    int maximum[MAX_PROCESSES][MAX_RESOURCES] = {

        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    int allocation[MAX_PROCESSES][MAX_RESOURCES] = {

        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int need[MAX_PROCESSES][MAX_RESOURCES];

    int safeSequence[MAX_PROCESSES];

    printf("\n");
    printf("============================================\n");
    printf("       MEMBER 3 DEADLOCK MODULE TEST\n");
    printf("============================================\n");

    printf("\n[1] Calculating Need Matrix...\n");

    calculateNeed(
        PROCESSES,
        RESOURCES,
        maximum,
        allocation,
        need
    );

    printNeedMatrix(
        PROCESSES,
        RESOURCES,
        need
    );

    printf("\n[2] Checking System Safety...\n");

    if (isSafeState(
            PROCESSES,
            RESOURCES,
            available,
            allocation,
            need,
            safeSequence)) {

        printf("\nSYSTEM STATUS: SAFE\n");

        printf("Safe Sequence: ");

        for (int i = 0; i < PROCESSES; i++) {

            printf("P%d", safeSequence[i]);

            if (i < PROCESSES - 1) {
                printf(" -> ");
            }
        }

        printf("\n");

    } else {

        printf("\nSYSTEM STATUS: UNSAFE\n");
    }

    printf("\n[3] Running Deadlock Detection...\n");

    int deadlocked[MAX_PROCESSES];

    int deadlockCount = detectDeadlock(
        PROCESSES,
        RESOURCES,
        available,
        allocation,
        need,
        deadlocked
    );

    printDeadlockedProcesses(
        PROCESSES,
        deadlocked
    );

    printf(
        "\nNumber of deadlocked processes: %d\n",
        deadlockCount
    );

    if (deadlockCount > 0) {

        printf("\n[4] Starting Recovery...\n");

        for (int i = 0; i < PROCESSES; i++) {

            if (deadlocked[i]) {

                recoverByProcessTermination(
                    i,
                    PROCESSES,
                    RESOURCES,
                    allocation,
                    available,
                    deadlocked
                );

                break;
            }
        }

    } else {

        printf("\n[4] Recovery not required.\n");
    }

    printf("\n[5] Final Resource Status\n");

    printAvailableResources(
        RESOURCES,
        available
    );

    printf("\n============================================\n");
    printf("          TEST COMPLETED\n");
    printf("============================================\n");

    return 0;
}
