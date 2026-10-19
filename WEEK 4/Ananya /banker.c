#include "banker.h"

#include <limits.h>

static bool dimensions_valid(const BankerState *state)
{
    return state != NULL &&
           state->process_count > 0 &&
           state->process_count <= BANKER_MAX_PROCESSES &&
           state->resource_count > 0 &&
           state->resource_count <= BANKER_MAX_RESOURCES;
}

bool banker_calculate_need(BankerState *state)
{
    size_t p, r;

    if (!dimensions_valid(state)) {
        return false;
    }

    for (p = 0; p < state->process_count; ++p) {
        for (r = 0; r < state->resource_count; ++r) {
            if (state->allocation[p][r] < 0 ||
                state->maximum[p][r] < 0 ||
                state->allocation[p][r] > state->maximum[p][r]) {
                return false;
            }
        }
    }

    for (p = 0; p < state->process_count; ++p) {
        for (r = 0; r < state->resource_count; ++r) {
            state->need[p][r] =
                state->maximum[p][r] - state->allocation[p][r];
        }
    }

    for (r = 0; r < state->resource_count; ++r) {
        if (state->available[r] < 0) {
            return false;
        }
    }

    return true;
}

bool banker_is_safe(const BankerState *state,
                    size_t safe_sequence[BANKER_MAX_PROCESSES],
                    size_t *sequence_length)
{
    int work[BANKER_MAX_RESOURCES];
    bool finish[BANKER_MAX_PROCESSES] = { false };
    size_t p, r, completed = 0;
    bool progress;

    if (sequence_length != NULL) {
        *sequence_length = 0;
    }
    if (!dimensions_valid(state) || safe_sequence == NULL ||
        sequence_length == NULL) {
        return false;
    }

    for (r = 0; r < state->resource_count; ++r) {
        if (state->available[r] < 0) {
            return false;
        }
        work[r] = state->available[r];
    }

    for (p = 0; p < state->process_count; ++p) {
        for (r = 0; r < state->resource_count; ++r) {
            if (state->allocation[p][r] < 0 ||
                state->need[p][r] < 0 ||
                state->maximum[p][r] < state->allocation[p][r]) {
                return false;
            }
        }
    }

    while (completed < state->process_count) {
        progress = false;

        for (p = 0; p < state->process_count; ++p) {
            bool can_finish = true;

            if (finish[p]) {
                continue;
            }

            for (r = 0; r < state->resource_count; ++r) {
                if (state->need[p][r] > work[r]) {
                    can_finish = false;
                    break;
                }
            }

            if (can_finish) {
                for (r = 0; r < state->resource_count; ++r) {
                    if (state->allocation[p][r] > INT_MAX - work[r]) {
                        return false;
                    }
                    work[r] += state->allocation[p][r];
                }
                finish[p] = true;
                safe_sequence[completed++] = p;
                progress = true;
            }
        }

        if (!progress) {
            *sequence_length = completed;
            return false;
        }
    }

    *sequence_length = completed;
    return true;
}

BankerRequestResult banker_request_resources(BankerState *state,
                                             size_t process_id,
                                             const int request[BANKER_MAX_RESOURCES])
{
    size_t r, sequence_length = 0;
    size_t safe_sequence[BANKER_MAX_PROCESSES];
    int old_available[BANKER_MAX_RESOURCES];

    if (!dimensions_valid(state) || request == NULL ||
        process_id >= state->process_count) {
        return BANKER_REQUEST_INVALID;
    }

    if (!banker_calculate_need(state)) {
        return BANKER_REQUEST_INVALID;
    }

    for (r = 0; r < state->resource_count; ++r) {
        if (request[r] < 0) {
            return BANKER_REQUEST_INVALID;
        }
        if (request[r] > state->need[process_id][r]) {
            return BANKER_REQUEST_EXCEEDS_NEED;
        }
        if (request[r] > state->available[r]) {
            return BANKER_REQUEST_NOT_AVAILABLE;
        }
    }

    for (r = 0; r < state->resource_count; ++r) {
        old_available[r] = state->available[r];
        state->available[r] -= request[r];
        state->allocation[process_id][r] += request[r];
        state->need[process_id][r] -= request[r];
    }

    if (banker_is_safe(state, safe_sequence, &sequence_length)) {
        return BANKER_REQUEST_GRANTED;
    }

    for (r = 0; r < state->resource_count; ++r) {
        state->available[r] = old_available[r];
        state->allocation[process_id][r] -= request[r];
        state->need[process_id][r] += request[r];
    }

    return BANKER_REQUEST_DENIED_UNSAFE;
}

const char *banker_request_result_string(BankerRequestResult result)
{
    switch (result) {
        case BANKER_REQUEST_GRANTED:
            return "Request granted: state remains safe.";
        case BANKER_REQUEST_DENIED_UNSAFE:
            return "Request denied: it would make the state unsafe.";
        case BANKER_REQUEST_EXCEEDS_NEED:
            return "Request denied: request exceeds the process's remaining need.";
        case BANKER_REQUEST_NOT_AVAILABLE:
            return "Request denied: resources are not currently available.";
        case BANKER_REQUEST_INVALID:
        default:
            return "Invalid request or invalid system state.";
    }
}
