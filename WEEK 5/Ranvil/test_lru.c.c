#include <assert.h>
#include <stdio.h>
#include "lru.h"

static void test_project_sample(void)
{
    const int pages[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3};
    SimulationResult result;
    int i;

    assert(simulate_lru(pages, 10, 3, &result) == 0);
    assert(result.total_references == 10);
    assert(result.total_hits == 2);
    assert(result.total_faults == 8);
    assert(result.hit_ratio > 19.99 && result.hit_ratio < 20.01);
    assert(result.fault_ratio > 79.99 && result.fault_ratio < 80.01);

    /* Each step must retain the requested page and a valid hit/fault flag. */
    for (i = 0; i < 10; ++i) {
        assert(result.steps[i].page == pages[i]);
        assert(result.steps[i].hit == 0 || result.steps[i].hit == 1);
    }
}

static void test_all_hits_after_first_load(void)
{
    const int pages[] = {5, 5, 5, 5};
    SimulationResult result;

    assert(simulate_lru(pages, 4, 1, &result) == 0);
    assert(result.total_hits == 3);
    assert(result.total_faults == 1);
    assert(result.steps[0].hit == 0);
    assert(result.steps[1].hit == 1);
}

static void test_one_frame_replacement(void)
{
    const int pages[] = {1, 2, 1};
    SimulationResult result;

    assert(simulate_lru(pages, 3, 1, &result) == 0);
    assert(result.total_hits == 0);
    assert(result.total_faults == 3);
    assert(result.steps[2].frames[0] == 1);
}

static void test_invalid_arguments(void)
{
    const int pages[] = {1, 2};
    SimulationResult result;

    assert(simulate_lru(NULL, 2, 2, &result) == -1);
    assert(simulate_lru(pages, 0, 2, &result) == -1);
    assert(simulate_lru(pages, 2, 0, &result) == -1);
    assert(simulate_lru(pages, 2, MAX_FRAMES + 1, &result) == -1);
    assert(simulate_lru(pages, MAX_PAGES + 1, 2, &result) == -1);
    assert(simulate_lru(pages, 2, 2, NULL) == -1);
}

static void print_result(const int pages[], int n, int frame_count,
                         const SimulationResult *result)
{
    int i, j;

    printf("\nLRU simulation (frames = %d)\n", frame_count);
    printf("Page\t");
    for (j = 0; j < frame_count; ++j) {
        printf("F%d\t", j + 1);
    }
    printf("Status\n");

    for (i = 0; i < n; ++i) {
        printf("%d\t", pages[i]);
        for (j = 0; j < frame_count; ++j) {
            if (result->steps[i].frames[j] == -1) {
                printf("-\t");
            } else {
                printf("%d\t", result->steps[i].frames[j]);
            }
        }
        printf("%s\n", result->steps[i].hit ? "Hit" : "Fault");
    }

    printf("References: %d | Hits: %d | Faults: %d\n",
           result->total_references, result->total_hits,
           result->total_faults);
    printf("Hit ratio: %.2f%% | Fault ratio: %.2f%%\n",
           result->hit_ratio, result->fault_ratio);
}

int main(void)
{
    const int pages[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3};
    SimulationResult result;

    test_project_sample();
    test_all_hits_after_first_load();
    test_one_frame_replacement();
    test_invalid_arguments();

    assert(simulate_lru(pages, 10, 3, &result) == 0);
    print_result(pages, 10, 3, &result);

    puts("\nAll LRU tests passed.");
    return 0;
}
