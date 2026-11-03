# Simulation and Comparative Analysis of Page Replacement Algorithms

## Project Overview
This project simulates and compares three page replacement algorithms used in operating systems: FIFO (First-In, First-Out), LRU (Least Recently Used), and Optimal Page Replacement.

The application is developed in C and uses GTK+ 3 to provide a graphical user interface. Users can enter a page-reference string, specify the number of memory frames, run an individual algorithm, or compare all three algorithms.

## Objectives
- Implement FIFO, LRU, and Optimal page replacement algorithms.
- Display page-frame changes for each page reference.
- Calculate page hits, page faults, hit ratio, and fault ratio.
- Compare the performance of all three algorithms using identical inputs.
- Provide a simple graphical interface for simulation.

## Technologies Used
- C programming language
- Linux / Ubuntu
- GTK+ 3
- GCC compiler
- Make build system

## Algorithms
- **FIFO:** Replaces the page that has been in memory the longest.
- **LRU:** Replaces the page that has not been used for the longest time.
- **Optimal:** Replaces the page whose next use is farthest in the future, or that will not be used again.

## Project Structure
- `Thrupthi/` – GUI implementation and application entry point.
- `ananya/` – FIFO algorithm implementation and tests.
- `Ranvil/` – LRU algorithm implementation and tests.
- `Sourabh/` – Optimal algorithm implementation and tests.
- `include/` – Shared data structures and function declarations.
- `tests/` – Test-case documentation.
- `docs/` – Algorithm comparison and project report.
- `Makefile` – Build and test commands.

## Build and Run
Install GTK+ 3 development packages if they are not already installed.

```bash
sudo apt install build-essential libgtk-3-dev
make
./page_replacement_simulator
```

## Run Automated Tests
```bash
make test
```

## Sample Comparison
For the page-reference string `7 0 1 2 0 3 0 4 2 3` with three frames:

| Algorithm | Hits | Faults | Hit Ratio | Fault Ratio |
|---|---:|---:|---:|---:|
| FIFO | 1 | 9 | 10% | 90% |
| LRU | 2 | 8 | 20% | 80% |
| Optimal | 4 | 6 | 40% | 60% |

These results show that Optimal produces the fewest page faults for this particular reference string.

## Team Contributions
The project is developed collaboratively. Refer to the individual algorithm folders and source files for implementation details.

## Conclusion
The application demonstrates how different page replacement strategies affect page hits and page faults. It provides step-by-step simulations and a comparative summary to support understanding of memory management in operating systems.