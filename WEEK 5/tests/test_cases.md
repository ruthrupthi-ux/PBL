# Test Cases – Page Replacement Algorithms

## 1. Objective
To verify the correctness of FIFO, LRU, and Optimal page replacement algorithms and the integrated graphical interface.

## 2. Test Input
- Page-reference string: `7 0 1 2 0 3 0 4 2 3`
- Number of frames: `3`

## 3. Algorithm Test Results

| Test ID | Algorithm | Expected Hits | Expected Faults | Result |
|---|---|---:|---:|---|
| TC01 | FIFO | 1 | 9 | PASS |
| TC02 | LRU | 2 | 8 | PASS |
| TC03 | Optimal | 4 | 6 | PASS |
| TC04 | Compare All | FIFO 1, LRU 2, Optimal 4 | FIFO 9, LRU 8, Optimal 6 | PASS |

## 4. Additional Optimal Algorithm Tests
The Optimal algorithm test suite also checks:
- A repeated-page input.
- A one-frame simulation.
- An empty page-reference string.
- Zero frames.
- A frame count greater than the supported maximum.
- Negative page numbers.
- A page-reference count above the supported maximum.
- Null page-array and result pointers.

All listed tests passed in the executed Optimal test suite.

## 5. GUI Validation
The GUI was launched and tested manually. FIFO, LRU, Optimal, and Compare All displayed the expected results for the sample input.

## 6. Conclusion
The individual algorithm tests passed, and the integrated GUI produced the expected sample results.