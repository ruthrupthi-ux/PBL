#include <string.h>
#include "optimal.h"

 
int simulate_optimal(const int pages[], int n, int frame_count,
                     SimulationResult *result)
{
    int frames[MAX_FRAMES];
    int i, j;

    if (pages == NULL || result == NULL ||
        n < 1 || n > MAX_PAGES ||
        frame_count < 1 || frame_count > MAX_FRAMES) {
        return -1;
    }

    for (i = 0; i < n; ++i) {
        if (pages[i] < 0) {
            return -1;
        }
    }

    memset(result, 0, sizeof(*result));

    for (j = 0; j < MAX_FRAMES; ++j) {
        frames[j] = -1;
    }

    result->total_references = n;

    for (i = 0; i < n; ++i) {
        int found = 0;
        int empty_frame = -1;

        for (j = 0; j < frame_count; ++j) {
            if (frames[j] == pages[i]) {
                found = 1;
                break;
            }

            if (frames[j] == -1 && empty_frame == -1) {
                empty_frame = j;
            }
        }

        result->steps[i].page = pages[i];

        if (found) {
            result->steps[i].hit = 1;
            ++result->total_hits;
        } else {
            result->steps[i].hit = 0;
            ++result->total_faults;

            if (empty_frame != -1) {
                frames[empty_frame] = pages[i];
            } else {
                int victim = -1;
                int farthest_next_use = -1;

                for (j = 0; j < frame_count; ++j) {
                    int k;
                    int next_use = n; 

                    for (k = i + 1; k < n; ++k) {
                        if (pages[k] == frames[j]) {
                            next_use = k;
                            break;
                        }
                    }

                    if (next_use == n) {
                        victim = j;
                        break;
                    }

                    if (next_use > farthest_next_use) {
                        farthest_next_use = next_use;
                        victim = j;
                    }
                }

                if (victim == -1) {
                    return -1;
                }

                frames[victim] = pages[i];
            }
        }
        for (j = 0; j < MAX_FRAMES; ++j) {
            result->steps[i].frames[j] = -1;
        }
        for (j = 0; j < frame_count; ++j) {
            result->steps[i].frames[j] = frames[j];
        }
    }

    result->hit_ratio =
        ((double)result->total_hits / (double)n) * 100.0;
    result->fault_ratio =
        ((double)result->total_faults / (double)n) * 100.0;

    return 0;
}
