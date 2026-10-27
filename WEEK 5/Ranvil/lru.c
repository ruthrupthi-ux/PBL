#include <stddef.h>
#include "lru.h"

int simulate_lru(const int pages[], int n, int frame_count,
                 SimulationResult *result)
{
    int last_used[MAX_FRAMES];
    int i, j;

    if (pages == NULL || result == NULL ||
        n <= 0 || n > MAX_PAGES ||
        frame_count <= 0 || frame_count > MAX_FRAMES) {
        return -1;
    }

    result->total_references = n;
    result->total_hits = 0;
    result->total_faults = 0;
    result->hit_ratio = 0.0;
    result->fault_ratio = 0.0;

    for (j = 0; j < frame_count; ++j) {
        last_used[j] = -1;
    }

    for (i = 0; i < n; ++i) {
        int hit_index = -1;
        int empty_index = -1;

        if (i == 0) {
            for (j = 0; j < frame_count; ++j) {
                result->steps[i].frames[j] = -1;
            }
        } else {
            for (j = 0; j < frame_count; ++j) {
                result->steps[i].frames[j] = result->steps[i - 1].frames[j];
            }
        }

        result->steps[i].page = pages[i];

        for (j = 0; j < frame_count; ++j) {
            if (result->steps[i].frames[j] == pages[i]) {
                hit_index = j;
                break;
            }
            if (empty_index == -1 && result->steps[i].frames[j] == -1) {
                empty_index = j;
            }
        }

        if (hit_index != -1) {
            result->steps[i].hit = 1;
            result->total_hits++;
            last_used[hit_index] = i;
        } else {
            int replace_index;

            result->steps[i].hit = 0;
            result->total_faults++;

            if (empty_index != -1) {
                replace_index = empty_index;
            } else {
                replace_index = 0;
                for (j = 1; j < frame_count; ++j) {
                    if (last_used[j] < last_used[replace_index]) {
                        replace_index = j;
                    }
                }
            }

            result->steps[i].frames[replace_index] = pages[i];
            last_used[replace_index] = i;
        }
        for (j = frame_count; j < MAX_FRAMES; ++j) {
            result->steps[i].frames[j] = -1;
        }
    }

    result->hit_ratio = ((double)result->total_hits / (double)n) * 100.0;
    result->fault_ratio = ((double)result->total_faults / (double)n) * 100.0;

    return 0;
}
