
#include <stdio.h>
#include <string.h>

#include "resource_manager.h"
#include "../Ananya /banker.h"

static int valid_state(const SystemState *state)
{
    return state != NULL &&
           state->process_count >= 1 &&
           state->process_count <= MAX_PROCESSES &&
           state->resource_count >= 1 &&
           state->resource_count <= MAX_RESOURCES;
}

void initialize_system(SystemState *state)
{
    if (state != NULL) {
        memset(state, 0, sizeof(*state));
    }
}

int create_processes(SystemState *state, int count)
{
    if (state == NULL || count < 1 || count > MAX_PROCESSES)
        return 0;

    if (state->process_count != 0)
        return 0;

    state->process_count = count;

    for (int p = 0; p < count; p++)
        state->status[p] = PROCESS_ACTIVE;

    return 1;
}

int create_resources(SystemState *state, int count,
                     const int totals[])
{
    if (state == NULL || totals == NULL ||
        count < 1 || count > MAX_RESOURCES)
        return 0;

    for (int r = 0; r < count; r++) {
        if (totals[r] < 0)
            return 0;

        int allocated = 0;
        for (int p = 0; p < state->process_count; p++)
            allocated += state->allocation[p][r];

        if (totals[r] < allocated)
            return 0;
    }

    for (int r = count; r < MAX_RESOURCES; r++) {
        for (int p = 0; p < state->process_count; p++) {
            if (state->allocation[p][r] != 0)
                return 0;
        }
    }

    state->resource_count = count;

    for (int r = 0; r < count; r++)
        state->total_resources[r] = totals[r];

    update_available_resources(state);
    update_need_matrix(state);

    return 1;
}

int set_maximum_demand(SystemState *state, int process_id,
                       const int demands[])
{
    if (!valid_state(state) || demands == NULL ||
        process_id < 0 || process_id >= state->process_count ||
        state->status[process_id] != PROCESS_ACTIVE)
        return 0;

    for (int r = 0; r < state->resource_count; r++) {
        if (demands[r] < state->allocation[process_id][r] ||
            demands[r] > state->total_resources[r])
            return 0;
    }

    for (int r = 0; r < state->resource_count; r++)
        state->max[process_id][r] = demands[r];

    update_need_matrix(state);
    return 1;
}

void update_need_matrix(SystemState *state)
{
    if (!valid_state(state))
        return;

    for (int p = 0; p < state->process_count; p++) {
        for (int r = 0; r < state->resource_count; r++) {
            state->need[p][r] =
                state->max[p][r] - state->allocation[p][r];
        }
    }
}

void update_available_resources(SystemState *state)
{
    if (!valid_state(state))
        return;

    for (int r = 0; r < state->resource_count; r++) {
        int allocated = 0;

        for (int p = 0; p < state->process_count; p++)
            allocated += state->allocation[p][r];

        state->available[r] = state->total_resources[r] - allocated;
    }
}

int allocate_resources(SystemState *state, int process_id,
                       const int resources[])
{
    /* Allocation uses the same safety checks as a resource request. */
    return request_resources(state, process_id, resources);
}

int request_resources(SystemState *state, int process_id,
                      const int requests[])
{
    if (!valid_state(state) || requests == NULL ||
        process_id < 0 || process_id >= state->process_count ||
        state->status[process_id] != PROCESS_ACTIVE)
        return 0;

    BankerState banker;
    memset(&banker, 0, sizeof(banker));

    banker.process_count = (size_t)state->process_count;
    banker.resource_count = (size_t)state->resource_count;

    for (int r = 0; r < state->resource_count; r++)
        banker.available[r] = state->available[r];

    for (int p = 0; p < state->process_count; p++) {
        for (int r = 0; r < state->resource_count; r++) {
            banker.allocation[p][r] = state->allocation[p][r];
            banker.maximum[p][r] = state->max[p][r];
        }
    }

    int request_copy[BANKER_MAX_RESOURCES] = {0};

    for (int r = 0; r < state->resource_count; r++) {
        if (requests[r] < 0)
            return 0;

        request_copy[r] = requests[r];
    }

    BankerRequestResult result =
        banker_request_resources(&banker, (size_t)process_id,
                                 request_copy);

    if (result != BANKER_REQUEST_GRANTED) {
        printf("%s\n", banker_request_result_string(result));
        return 0;
    }

    /* Commit the Banker's approved state to the shared system. */
    for (int p = 0; p < state->process_count; p++) {
        for (int r = 0; r < state->resource_count; r++) {
            state->allocation[p][r] = banker.allocation[p][r];
        }
    }

    for (int r = 0; r < state->resource_count; r++)
        state->available[r] = banker.available[r];

    update_need_matrix(state);

    for (int r = 0; r < state->resource_count; r++)
        state->request[process_id][r] = 0;

    return 1;
}

void display_system_status(const SystemState *state)
{
    if (!valid_state(state))
        return;

    printf("\n========== SYSTEM STATUS ==========\n");
    printf("Processes: %d\n", state->process_count);
    printf("Resource types: %d\n", state->resource_count);

    printf("Total:     ");
    for (int r = 0; r < state->resource_count; r++)
        printf("%d ", state->total_resources[r]);

    printf("\nAvailable: ");
    for (int r = 0; r < state->resource_count; r++)
        printf("%d ", state->available[r]);

    printf("\n\nProcess\tStatus\tAllocation\tMaximum\t\tNeed\n");

    for (int p = 0; p < state->process_count; p++) {
        printf("P%d\t%d\t", p, state->status[p]);

        for (int r = 0; r < state->resource_count; r++)
            printf("%d ", state->allocation[p][r]);

        printf("\t\t");

        for (int r = 0; r < state->resource_count; r++)
            printf("%d ", state->max[p][r]);

        printf("\t\t");

        for (int r = 0; r < state->resource_count; r++)
            printf("%d ", state->need[p][r]);

        printf("\n");
    }

    printf("===================================\n");
}

