#ifndef LRU_H
#define LRU_H

#include "page_replacement.h"

/* Returns 0 on success; -1 if any argument or limit is invalid. */
int simulate_lru(const int pages[], int n, int frame_count,
                 SimulationResult *result);

#endif
