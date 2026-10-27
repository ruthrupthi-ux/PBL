# Comparative Analysis of Page Replacement Algorithms

## 1. Introduction
Page replacement algorithms determine which page should be removed from memory when a page fault occurs and all available frames are occupied. This project compares FIFO, LRU, and Optimal page replacement algorithms.

## 2. Algorithms

### FIFO – First-In, First-Out
FIFO replaces the page that entered memory earliest. It is simple to implement, but it may remove a page that will be needed soon.

### LRU – Least Recently Used
LRU replaces the page that has not been accessed for the longest time. It uses recent access history to make replacement decisions.

### Optimal Page Replacement
Optimal replaces the page whose next use is farthest in the future, or that will not be used again. It is useful as a theoretical benchmark for comparing other algorithms.

## 3. Experimental Input
- Page-reference string: `7 0 1 2 0 3 0 4 2 3`
- Number of frames: `3`
- Total page references: `10`

## 4. Results

| Metric | FIFO | LRU | Optimal |
|---|---:|---:|---:|
| Page hits | 1 | 2 | 4 |
| Page faults | 9 | 8 | 6 |
| Hit ratio | 10% | 20% | 40% |
| Fault ratio | 90% | 80% | 60% |

## 5. Interpretation
For this test input, FIFO produces nine page faults. LRU reduces the faults to eight, while Optimal produces six faults.

Optimal achieves the highest hit ratio and the lowest fault ratio in this experiment. This is consistent with its strategy of replacing the page whose next use is farthest in the future.

These findings apply to the tested reference string and three-frame configuration; different inputs may produce different results.

## 6. Conclusion
The experiment demonstrates that the choice of page replacement algorithm affects memory performance. The comparison feature allows users to observe these differences using identical page references and frame counts.