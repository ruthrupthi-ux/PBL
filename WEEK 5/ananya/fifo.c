#include "fifo.h"

int simulate_fifo(const int pages[], int n, int frame_count, SimulationResult *result)
{
    int frames[MAX_FRAMES];
    int next_to_replace = 0;
    int i, j;

    if (result == 0 || pages == 0 || n <= 0 || n > MAX_PAGES ||
        frame_count <= 0 || frame_count > MAX_FRAMES)
        return -1;

    /* -1 marks an empty frame, so page IDs must be non-negative. */
    for (i = 0; i < n; ++i)
        if (pages[i] < 0)
            return -1;

    result->total_references = n;
    result->total_hits = 0;
    result->total_faults = 0;
    result->hit_ratio = 0.0;
    result->fault_ratio = 0.0;

    for (j = 0; j < MAX_FRAMES; ++j)
        frames[j] = -1;

    for (i = 0; i < n; ++i) {
        int hit = 0;
        int empty = -1;
        for (j = 0; j < frame_count; ++j) {
            if (frames[j] == pages[i]) {
                hit = 1;
                break;
            }
            if (frames[j] == -1 && empty == -1)
                empty = j;
        }
        if (hit) {
            ++result->total_hits;
        } else {
            ++result->total_faults;
            if (empty != -1) {
                frames[empty] = pages[i];
            } else {
                frames[next_to_replace] = pages[i];
                next_to_replace = (next_to_replace + 1) % frame_count;
            }
        }
        result->steps[i].page = pages[i];
        result->steps[i].hit = hit;
        for (j = 0; j < MAX_FRAMES; ++j)
            result->steps[i].frames[j] = (j < frame_count) ? frames[j] : -1;
    }
    result->hit_ratio = (double)result->total_hits / n;
    result->fault_ratio = (double)result->total_faults / n;
    return 0;
}
