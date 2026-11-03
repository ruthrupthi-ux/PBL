#include <stdio.h>
#include <stdlib.h>
#include "optimal.h"

static int check(int condition, const char *description)
{
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", description);
        return 0;
    }

    printf("PASS: %s\n", description);
    return 1;
}

static void print_result(const SimulationResult *result, int frame_count)
{
    int i, j;

    printf("\nOptimal Algorithm - Step-by-Step Simulation\n");
    printf("Page\t");
    for (j = 0; j < frame_count; ++j) {
        printf("F%d\t", j + 1);
    }
    printf("Status\n");

    for (i = 0; i < result->total_references; ++i) {
        printf("%d\t", result->steps[i].page);

        for (j = 0; j < frame_count; ++j) {
            if (result->steps[i].frames[j] == -1) {
                printf("-\t");
            } else {
                printf("%d\t", result->steps[i].frames[j]);
            }
        }

        printf("%s\n", result->steps[i].hit ? "Hit" : "Fault");
    }

    printf("\nTotal references: %d\n", result->total_references);
    printf("Page hits:        %d\n", result->total_hits);
    printf("Page faults:      %d\n", result->total_faults);
    printf("Hit ratio:        %.2f%%\n", result->hit_ratio);
    printf("Fault ratio:      %.2f%%\n", result->fault_ratio);
}

int main(void)
{
    static SimulationResult result;
    const int sample[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3};
    const int repeated_pages[] = {5, 5, 5, 5};
    const int one_frame_pages[] = {1, 2, 1};
    const int negative_page[] = {1, -2, 3};
    int all_passed = 1;

    if (simulate_optimal(sample, 10, 3, &result) != 0) {
        fprintf(stderr, "Sample simulation unexpectedly failed.\n");
        return EXIT_FAILURE;
    }

    print_result(&result, 3);

    all_passed &= check(result.total_references == 10,
                        "sample has 10 page references");
    all_passed &= check(result.total_hits == 4,
                        "sample has 4 page hits");
    all_passed &= check(result.total_faults == 6,
                        "sample has 6 page faults");
    all_passed &= check(result.total_hits + result.total_faults == 10,
                        "hits plus faults equals total references");
    all_passed &= check(result.hit_ratio > 39.99 && result.hit_ratio < 40.01,
                        "sample hit ratio is 40 percent");
    all_passed &= check(result.fault_ratio > 59.99 && result.fault_ratio < 60.01,
                        "sample fault ratio is 60 percent");

    all_passed &= check(simulate_optimal(repeated_pages, 4, 2, &result) == 0,
                        "repeated-page input is accepted");
    all_passed &= check(result.total_hits == 3 && result.total_faults == 1,
                        "repeated-page input has 3 hits and 1 fault");

    all_passed &= check(simulate_optimal(one_frame_pages, 3, 1, &result) == 0,
                        "one-frame input is accepted");
    all_passed &= check(result.total_hits == 0 && result.total_faults == 3,
                        "one-frame input has 0 hits and 3 faults");

    all_passed &= check(simulate_optimal(sample, 0, 3, &result) == -1,
                        "empty reference string is rejected");
    all_passed &= check(simulate_optimal(sample, 10, 0, &result) == -1,
                        "zero frames are rejected");
    all_passed &= check(simulate_optimal(sample, 10, MAX_FRAMES + 1,
                                         &result) == -1,
                        "frame count above MAX_FRAMES is rejected");
    all_passed &= check(simulate_optimal(negative_page, 3, 2, &result) == -1,
                        "negative page number is rejected");
    all_passed &= check(simulate_optimal(sample, MAX_PAGES + 1, 3,
                                         &result) == -1,
                        "reference count above MAX_PAGES is rejected");
    all_passed &= check(simulate_optimal(NULL, 10, 3, &result) == -1,
                        "null page array is rejected");
    all_passed &= check(simulate_optimal(sample, 10, 3, NULL) == -1,
                        "null result pointer is rejected");

    if (!all_passed) {
        fprintf(stderr, "\nOne or more tests failed.\n");
        return EXIT_FAILURE;
    }

    printf("\nAll Optimal algorithm tests passed.\n");
    return EXIT_SUCCESS;
}
