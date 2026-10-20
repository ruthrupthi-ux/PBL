#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "../Include/common.h"

void initialize_system(SystemState *state);

int create_processes(SystemState *state, int count);

int create_resources(SystemState *state, int count,
                     const int totals[]);

int set_maximum_demand(SystemState *state, int process_id,
                       const int demands[]);

int allocate_resources(SystemState *state, int process_id,
                       const int resources[]);

int request_resources(SystemState *state, int process_id,
                      const int requests[]);

void update_need_matrix(SystemState *state);
void update_available_resources(SystemState *state);

void display_system_status(const SystemState *state);

#endif