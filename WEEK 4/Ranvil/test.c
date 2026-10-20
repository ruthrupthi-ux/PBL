#include <stdio.h>

#include "deadlock.h"
#include "recovery.h"

#define PROCESSES 3
#define RESOURCES 2

int main() {

    int available[MAX_RESOURCES] = {
        0, 0
    };

    int allocation[MAX_PROCESSES][MAX_RESOURCES] = {

        {1, 0},
        {0, 1},
        {0, 0}
    };

    int request[MAX_PROCESSES][MAX_RESOURCES] = {

        {0, 1},
        {1, 0},
        {0, 0}
    };

    int deadlocked[MAX_PROCESSES];

    printf("\n");
    printf("============================================\n");
    printf("       DEADLOCK DETECTION & RECOVERY\n");
    printf("              MEMBER 3 TEST\n");
    printf("============================================\n");

    printf("\n[1] INITIAL RESOURCE STATUS\n");

    printAvailableResources(
        RESOURCES,
        available
    );

    printf("\n[2] RUNNING DEADLOCK DETECTION...\n");

    int deadlockCount = detectDeadlock(
        PROCESSES,
        RESOURCES,
        available,
        allocation,
        request,
        deadlocked
    );

    printDeadlockedProcesses(
        PROCESSES,
        deadlocked
    );

    printf(
        "\nTotal deadlocked processes: %d\n",
        deadlockCount
    );

    if (deadlockCount > 0) {

        printf("\n[3] STARTING DEADLOCK RECOVERY...\n");

        recoverByProcessTermination(
            0,
            PROCESSES,
            RESOURCES,
            allocation,
            available,
            deadlocked
        );
    }

    printf("\n[4] RESOURCE STATUS AFTER RECOVERY\n");

    printAvailableResources(
        RESOURCES,
        available
    );

    printf("\n[5] RUNNING DEADLOCK DETECTION AGAIN...\n");

    deadlockCount = detectDeadlock(
        PROCESSES,
        RESOURCES,
        available,
        allocation,
        request,
        deadlocked
    );

    printDeadlockedProcesses(
        PROCESSES,
        deadlocked
    );

    printf(
        "\nRemaining deadlocked processes: %d\n",
        deadlockCount
    );

    printf("\n============================================\n");
    printf("              TEST COMPLETED\n");
    printf("============================================\n");

    return 0;
}
