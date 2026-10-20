#include <stdio.h>
#include "banker.h"

void calculateNeed(
    int processes,
    int resources,
    int maximum[MAX_PROCESSES][MAX_RESOURCES],
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int need[MAX_PROCESSES][MAX_RESOURCES]
) {
    for (int i = 0; i < processes; i++) {
        for (int j = 0; j < resources; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];

            if (need[i][j] < 0) {
                need[i][j] = 0;
            }
        }
    }
}

void printNeedMatrix(
    int processes,
    int resources,
    int need[MAX_PROCESSES][MAX_RESOURCES]
) {
    printf("\n========== NEED MATRIX ==========\n");

    printf("        ");
    for (int j = 0; j < resources; j++) {
        printf("R%-6d", j);
    }
    printf("\n");

    for (int i = 0; i < processes; i++) {
        printf("P%-6d", i);

        for (int j = 0; j < resources; j++) {
            printf("%-7d", need[i][j]);
        }

        printf("\n");
    }

    printf("=================================\n");
}

int isSafeState(
    int processes,
    int resources,
    int available[MAX_RESOURCES],
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int need[MAX_PROCESSES][MAX_RESOURCES],
    int safeSequence[MAX_PROCESSES]
) {
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES];

    for (int i = 0; i < resources; i++) {
        work[i] = available[i];
    }

    for (int i = 0; i < processes; i++) {
        finish[i] = 0;
    }

    int count = 0;

    while (count < processes) {
        int found = 0;

        for (int i = 0; i < processes; i++) {

            if (finish[i] == 0) {

                int canFinish = 1;

                for (int j = 0; j < resources; j++) {
                    if (need[i][j] > work[j]) {
                        canFinish = 0;
                        break;
                    }
                }

                if (canFinish) {

                    for (int j = 0; j < resources; j++) {
                        work[j] += allocation[i][j];
                    }

                    safeSequence[count] = i;
                    count++;

                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if (!found) {
            return 0;
        }
    }

    return 1;
}
