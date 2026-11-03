#ifndef LRU_H
#define LRU_H

#include "page_replacement.h"

int simulate_lru(const int pages[], int n, int frame_count,
                 SimulationResult *result);

#endif
