# Team Leader – GUI Integration

## Responsibilities
The team leader is responsible for integrating the FIFO, LRU, and Optimal page replacement algorithms into a single graphical application.

## Files
- `main.c` – Initializes the GTK application.
- `gui.c` – Implements the graphical interface, accepts user inputs, runs simulations, and displays results.
- `gui.h` – Declares the GUI function used by the application.

## Features
- Accepts a page-reference string.
- Allows the user to select the number of frames.
- Supports FIFO, LRU, and Optimal algorithms individually.
- Provides a Compare All option.
- Displays the frame contents after each page reference.
- Shows page hits, page faults, hit ratio, and fault ratio.
- Provides a Reset button to clear the inputs and results.

## Build and Run
Run these commands from the project root:

```bash
make
./page_replacement_simulator
```

## Integration Testing
The GUI was tested with the page-reference string `7 0 1 2 0 3 0 4 2 3` and three frames.

| Algorithm | Hits | Faults |
|---|---:|---:|
| FIFO | 1 | 9 |
| LRU | 2 | 8 |
| Optimal | 4 | 6 |

The Compare All feature displays the results of all three algorithms in one output.

## Conclusion
The GUI integrates the algorithm modules into one application and enables users to observe and compare their page replacement behavior.