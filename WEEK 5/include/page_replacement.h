#ifndef PAGE_REPLACEMENT_H
#define PAGE_REPLACEMENT_H

#define MAX_PAGES 1000
#define MAX_FRAMES 100


typedef struct {
    int page;
    int frames[MAX_FRAMES];
    int hit; 
} PageStep;


typedef struct {
    PageStep steps[MAX_PAGES];
    int total_references;
    int total_hits;
    int total_faults;
    double hit_ratio;  
    double fault_ratio; 
} SimulationResult;

int simulate_fifo(const int pages[], int n, int frame_count,
                  SimulationResult *result);
int simulate_lru(const int pages[], int n, int frame_count,
                 SimulationResult *result);
int simulate_optimal(const int pages[], int n, int frame_count,
                     SimulationResult *result);

#endif
