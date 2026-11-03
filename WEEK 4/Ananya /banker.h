#ifndef BANKER_H
#define BANKER_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Limits keep the module portable and easy to integrate into the team project. */
#define BANKER_MAX_PROCESSES 20
#define BANKER_MAX_RESOURCES 10

typedef struct {
    size_t process_count;
    size_t resource_count;
    int available[BANKER_MAX_RESOURCES];
    int allocation[BANKER_MAX_PROCESSES][BANKER_MAX_RESOURCES];
    int maximum[BANKER_MAX_PROCESSES][BANKER_MAX_RESOURCES];
    int need[BANKER_MAX_PROCESSES][BANKER_MAX_RESOURCES];
} BankerState;

typedef enum {
    BANKER_REQUEST_GRANTED = 0,
    BANKER_REQUEST_DENIED_UNSAFE,
    BANKER_REQUEST_EXCEEDS_NEED,
    BANKER_REQUEST_NOT_AVAILABLE,
    BANKER_REQUEST_INVALID
} BankerRequestResult;

/*
 * Recalculate Need = Maximum - Allocation.
 * Returns false if dimensions or matrix values are invalid.
 */
bool banker_calculate_need(BankerState *state);

/* Returns true when every process can finish; writes a safe sequence on success. */
bool banker_is_safe(const BankerState *state,
                   size_t safe_sequence[BANKER_MAX_PROCESSES],
                   size_t *sequence_length);

/* Validate and tentatively apply a request; rolls back if it makes state unsafe. */
BankerRequestResult banker_request_resources(BankerState *state,
                                             size_t process_id,
                                             const int request[BANKER_MAX_RESOURCES]);

const char *banker_request_result_string(BankerRequestResult result);

#ifdef __cplusplus
}
#endif

#endif
