#include <assert.h>
#include <stdio.h>
#include "fifo.h"

static void check(const int *pages, int n, int count, int hits, int faults)
{
    SimulationResult result;
    int i;
    assert(simulate_fifo(pages, n, count, &result) == 0);
    assert(result.total_references == n);
    assert(result.total_hits == hits);
    assert(result.total_faults == faults);
    assert(result.hit_ratio == (double)hits / n);
    assert(result.fault_ratio == (double)faults / n);
    for (i = 0; i < n; ++i) assert(result.steps[i].page == pages[i]);
}

int main(void)
{
    const int sample[] = {7,0,1,2,0,3,0,4,2,3};
    const int repeated[] = {5,5,5,5};
    const int cycling[] = {1,2,3,1,4,2,5};
    const int one[] = {9,8,9};
    SimulationResult result;
    const int invalid[] = {-1};
    int i;
    check(sample, 10, 3, 1, 9);
    check(repeated, 4, 3, 3, 1);
    check(cycling, 7, 3, 2, 5);
    check(one, 3, 1, 0, 3);
    assert(simulate_fifo(sample, 0, 3, &result) == -1);
    assert(simulate_fifo(sample, 10, 0, &result) == -1);
    assert(simulate_fifo(sample, 10, MAX_FRAMES + 1, &result) == -1);
    assert(simulate_fifo(sample, MAX_PAGES + 1, 3, &result) == -1);
    assert(simulate_fifo(NULL, 10, 3, &result) == -1);
    assert(simulate_fifo(invalid, 1, 3, &result) == -1);
    assert(simulate_fifo(sample, 10, 3, NULL) == -1);
    assert(simulate_fifo(sample, 10, 3, &result) == 0);
    assert(result.steps[0].frames[0] == 7 && result.steps[0].frames[1] == -1);
    assert(result.steps[3].frames[0] == 2 && result.steps[3].frames[1] == 0);
    assert(result.steps[4].hit == 1);
    assert(result.steps[9].frames[0] == 4 && result.steps[9].frames[1] == 2 && result.steps[9].frames[2] == 3);
    printf("Page | Frames | Status\n");
    for (i = 0; i < result.total_references; ++i) {
        int j;
        printf("%4d |", result.steps[i].page);
        for (j = 0; j < 3; ++j) printf(" %2d", result.steps[i].frames[j]);
        printf(" | %s\n", result.steps[i].hit ? "Hit" : "Fault");
    }
    printf("Hits: %d, Faults: %d, Hit ratio: %.2f%%, Fault ratio: %.2f%%\n",
           result.total_hits, result.total_faults,
           result.hit_ratio * 100.0, result.fault_ratio * 100.0);
    puts("All FIFO tests passed.");
    return 0;
}
