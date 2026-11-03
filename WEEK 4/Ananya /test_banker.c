#include "banker.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static BankerState sample_state(void)
{
    BankerState state;
    memset(&state, 0, sizeof(state));

    state.process_count = 5;
    state.resource_count = 3;

    state.available[0] = 3;
    state.available[1] = 3;
    state.available[2] = 2;

    {
        const int allocation[5][3] = {
            {0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}
        };
        const int maximum[5][3] = {
            {7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}
        };
        size_t p, r;
        for (p = 0; p < 5; ++p) {
            for (r = 0; r < 3; ++r) {
                state.allocation[p][r] = allocation[p][r];
                state.maximum[p][r] = maximum[p][r];
            }
        }
    }
    return state;
}

int main(void)
{
    BankerState state = sample_state();
    size_t sequence[BANKER_MAX_PROCESSES];
    size_t length = 0;
    const int safe_request[BANKER_MAX_RESOURCES] = {1, 0, 2};
    const int excessive_request[BANKER_MAX_RESOURCES] = {9, 0, 0};
    const int unavailable_request[BANKER_MAX_RESOURCES] = {0, 4, 0};
    BankerRequestResult result;

    assert(banker_calculate_need(&state));
    assert(state.need[0][0] == 7);
    assert(state.need[0][1] == 4);
    assert(state.need[0][2] == 3);

    assert(banker_is_safe(&state, sequence, &length));
    assert(length == 5);
    printf("Initial state is SAFE. Safe sequence: ");
    for (size_t i = 0; i < length; ++i) {
        printf("P%zu%s", sequence[i], (i + 1 == length) ? "\n" : " -> ");
    }

    result = banker_request_resources(&state, 1, safe_request);
    assert(result == BANKER_REQUEST_GRANTED);
    puts(banker_request_result_string(result));

    result = banker_request_resources(&state, 0, excessive_request);
    assert(result == BANKER_REQUEST_EXCEEDS_NEED);
    puts(banker_request_result_string(result));

    result = banker_request_resources(&state, 0, unavailable_request);
    assert(result == BANKER_REQUEST_EXCEEDS_NEED ||
           result == BANKER_REQUEST_NOT_AVAILABLE);
    puts(banker_request_result_string(result));

    puts("All Banker module tests passed.");
    return 0;
}
