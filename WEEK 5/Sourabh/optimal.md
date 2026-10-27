# Optimal Page Replacement Algorithm

## 1. Objective
Implement and test the Optimal Page Replacement Algorithm using C.

The Optimal algorithm replaces the page whose next use is farthest in the future. If a page will not be used again, it can be selected for replacement.

## 2. Files and Their Purpose

| File | Description |
|---|---|
| `optimal.c` | Implements the Optimal Page Replacement Algorithm. |
| `optimal.h` | Provides the header interface for the Optimal module. |
| `test_optimal.c` | Tests the algorithm with sample inputs and invalid inputs. |
| `include/page_replacement.h` | Contains the common data structures and function declarations shared by the team. |

## 3. Algorithm Workflow

1. Validate the input page-reference string and frame count.
2. Initialize all frames as empty.
3. Read each page reference.
4. If the page is already in a frame, record a page hit.
5. If the page is not in a frame, record a page fault.
6. If an empty frame is available, load the page into it.
7. If all frames are full, look ahead in the reference string.
8. Replace the page whose next use is farthest in the future.
9. Store the frame states and hit/fault status.
10. Calculate total hits, total faults, hit ratio, and fault ratio.

## 4. Sample Input

- **Page-reference string:** `7 0 1 2 0 3 0 4 2 3`
- **Number of references:** `10`
- **Number of frames:** `3`

## 5. Simulation Results

| Page | Frame 1 | Frame 2 | Frame 3 | Status |
|---:|---:|---:|---:|---|
| 7 | 7 | - | - | Fault |
| 0 | 7 | 0 | - | Fault |
| 1 | 7 | 0 | 1 | Fault |
| 2 | 2 | 0 | 1 | Fault |
| 0 | 2 | 0 | 1 | Hit |
| 3 | 2 | 0 | 3 | Fault |
| 0 | 2 | 0 | 3 | Hit |
| 4 | 2 | 4 | 3 | Fault |
| 2 | 2 | 4 | 3 | Hit |
| 3 | 2 | 4 | 3 | Hit |

### Final Statistics

| Metric | Result |
|---|---:|
| Total references | 10 |
| Page hits | 4 |
| Page faults | 6 |
| Hit ratio | 40.00% |
| Fault ratio | 60.00% |

## 6. Test Cases

The following tests passed in the current test run:

- Sample input produces 4 hits and 6 faults.
- Repeated pages produce 3 hits and 1 fault.
- One-frame input produces 0 hits and 3 faults.
- Empty reference strings are rejected.
- Zero frames are rejected.
- Frame counts exceeding `MAX_FRAMES` are rejected.
- Negative page numbers are rejected.
- Reference counts exceeding `MAX_PAGES` are rejected.
- Null page arrays are rejected.
- Null result pointers are rejected.

## 7. Compilation and Execution

Run these commands from the project root:

```bash
gcc -Wall -Wextra -Werror -std=c11 \
    Member3_Optimal/optimal.c \
    Member3_Optimal/test_optimal.c \
    -o test_optimal

./test_optimal
```

Expected final message:

```text
All Optimal algorithm tests passed.
```

## 8. Comparison with FIFO and LRU

The current `test_optimal.c` tests only the Optimal algorithm. It does not compare FIFO, LRU, and Optimal.

Once the FIFO and LRU implementations are ready, all three algorithms should be tested using the same page-reference string and frame count. Their page hits, page faults, hit ratios, and fault ratios should then be recorded in the comparison report.

## 9. Conclusion

The Optimal Page Replacement Algorithm has been implemented and tested successfully. The current test suite passes all included test cases. The module is ready for integration with the team's other algorithms, provided the common header matches the team's final version.
