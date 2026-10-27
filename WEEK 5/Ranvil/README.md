# LRU Page Replacement

## Files
- `lru.c`: implementation of Least Recently Used page replacement.
- `lru.h`: public declaration for the LRU function.
- `test_lru.c`: test program, including the project sample and invalid-input tests.

## Shared interface
The module uses `SimulationResult` and the constants `MAX_PAGES` / `MAX_FRAMES`
from `include/page_replacement.h`. Keep the shared header agreed by the team in
`WEEK5/include/page_replacement.h`; do not create a separate competing definition.

Function:
```c
int simulate_lru(const int pages[], int n, int frame_count,
                 SimulationResult *result);
```
It returns `0` on success and `-1` for invalid arguments or unsupported sizes.
Each `steps[i]` stores the requested page, the frame state after that request,
and `hit` (`1` for hit, `0` for page fault). Empty frame slots are represented
by `-1`. `hit_ratio` and `fault_ratio` are percentages (for example, `20.0`
means 20%).

## Compile and run on Ubuntu
From the `WEEK5` repository root:
```bash
gcc -Wall -Wextra -Wpedantic -std=c11 -Iinclude \
    Member2_LRU/lru.c Member2_LRU/test_lru.c -o test_lru
./test_lru
```

## Project sample
Reference string: `7 0 1 2 0 3 0 4 2 3`, frames: `3`.

Expected LRU summary:
- References: 10
- Hits: 2
- Faults: 8
- Hit ratio: 20.00%
- Fault ratio: 80.00%

## How LRU works
1. Check whether the requested page is already in a frame.
2. On a hit, update that frame's last-used position.
3. On a fault, use an empty frame if one exists.
4. Otherwise replace the resident page whose last use is oldest.
5. Save the frame state and hit/fault status for every reference.

## Test coverage
- Project sample reference string.
- Repeated requests that become hits after the first load.
- One-frame replacement behavior.
- Null arguments, zero references, invalid frame count, and over-limit sizes.

## Integration notes
The GUI should call `simulate_lru(...)` and inspect `result.steps[i]` for
step-by-step output. Compile with `-Iinclude` so `page_replacement.h` is found.
