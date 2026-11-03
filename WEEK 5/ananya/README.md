# Member 1 — FIFO Page Replacement

This folder implements **First-In, First-Out (FIFO)** page replacement in C for the Week 5 Operating Systems project. It uses the team's shared `../include/page_replacement.h` interface.

## Files
- `fifo.c`: implementation of `simulate_fifo()`
- `fifo.h`: FIFO declaration and shared header inclusion
- `test_fifo.c`: standalone Linux tests and sample step-by-step output
- `README.md`: build and integration instructions

## Linux compilation and testing
From the `WEEK5` directory:

```bash
gcc -std=c11 -Wall -Wextra -Werror -pedantic Member1_FIFO/fifo.c Member1_FIFO/test_fifo.c -o test_fifo
./test_fifo
```

Or from `Member1_FIFO`:

```bash
gcc -std=c11 -Wall -Wextra -Werror -pedantic fifo.c test_fifo.c -o test_fifo
./test_fifo
```

Expected sample: 10 references, 1 hit, 9 faults, hit ratio 10%, fault ratio 90%.

## Integration
Compile `Member1_FIFO/fifo.c` with the team leader's main program and the other algorithm source files. Do **not** compile `test_fifo.c` into the final program because it contains its own `main()`.

`simulate_fifo(pages, n, frame_count, &result)` returns `0` on success and `-1` on invalid input. It supports 1–1000 page references and 1–100 frames. `-1` in a frame means empty; page IDs must be non-negative (the shared interface uses `-1` for empty frames). The `hit_ratio` and `fault_ratio` fields are fractions from 0 to 1; multiply by 100 for percentages.

FIFO preserves insertion order on a hit. It fills empty frames first, then replaces the oldest resident page in circular order. The function does not print: the team leader's UI displays the recorded steps.
