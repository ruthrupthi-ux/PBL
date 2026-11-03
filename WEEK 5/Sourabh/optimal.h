#ifndef OPTIMAL_H
#define OPTIMAL_H

#include "../include/page_replacement.h"

int simulate_optimal(const int pages[], int n, int frame_count,
                     SimulationResult *result);

#endif