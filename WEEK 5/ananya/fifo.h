#ifndef FIFO_H
#define FIFO_H
#include "../include/page_replacement.h"
/* Returns 0 on success, -1 for invalid arguments. */
int simulate_fifo(const int pages[], int n, int frame_count, SimulationResult *result);
#endif
