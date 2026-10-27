# Project Report: Simulation and Comparative Analysis of Page Replacement Algorithms

## 1. Abstract
This project implements and compares three page replacement algorithms: FIFO, LRU, and Optimal. A graphical user interface developed using GTK+ 3 allows users to enter page references, specify the number of frames, execute simulations, and compare algorithm performance. Results include frame states, page hits, page faults, hit ratio, and fault ratio.

## 2. Introduction
Operating systems use virtual memory to allow processes to operate with limited physical memory. When a required page is not present in memory, a page fault occurs. If all frames are occupied, a page replacement algorithm selects a page to remove. Different algorithms may produce different numbers of page faults for the same reference sequence.

## 3. Objectives
- Implement FIFO, LRU, and Optimal algorithms in C.
- Display page replacement steps through a graphical interface.
- Calculate page hits, page faults, hit ratio, and fault ratio.
- Compare all three algorithms using identical inputs.
- Validate implementations through automated tests.

## 4. Technologies Used
- C programming language
- Linux / Ubuntu
- GCC compiler
- GTK+ 3
- Make build system

## 5. System Design
The application contains three algorithm modules and a GUI integration module. The shared header file defines common data structures and function declarations.

The GUI receives the input sequence and frame count, invokes the selected algorithm, and displays the resulting frame states and performance metrics. The Compare All option runs all three algorithms using the same input.

## 6. Methodology
1. Accept a page-reference sequence from the user.
2. Accept the number of memory frames.
3. Select FIFO, LRU, Optimal, or Compare All.
4. Simulate the page-reference sequence.
5. record frame contents and identify hits and faults.
6. Calculate hit and fault ratios.
7. Display and compare the results.

## 7. Experimental Results
For the page-reference sequence `7 0 1 2 0 3 0 4 2 3`, using three frames:

| Algorithm | Hits | Faults | Hit Ratio | Fault Ratio |
|---|---:|---:|---:|---:|
| FIFO | 1 | 9 | 10% | 90% |
| LRU | 2 | 8 | 20% | 80% |
| Optimal | 4 | 6 | 40% | 60% |

The GUI results and automated tests matched these expected sample results.

## 8. Discussion
For the selected input, Optimal produces the fewest page faults, followed by LRU and FIFO. Optimal uses future page-reference knowledge, making it a theoretical benchmark rather than a directly implementable general-purpose strategy in a real operating system.

The experiment illustrates how replacement strategies influence page-fault frequency. The results are specific to the selected input and frame count.

## 9. Testing
The FIFO, LRU, and Optimal test suites were executed using `make test`. All three test suites passed. The GUI was also manually tested for individual algorithm execution and the Compare All feature.

## 10. Limitations
- The comparison uses user-provided page-reference sequences.
- The displayed results depend on the chosen input and frame count.
- Optimal requires knowledge of future page references.
- The current application focuses on simulation rather than measuring real operating-system memory performance.

## 11. Conclusion
The project successfully integrates FIFO, LRU, and Optimal page replacement algorithms into a graphical application. It demonstrates their step-by-step behavior and compares page hits, page faults, and ratios. The project supports learning about memory management and the performance differences between page replacement strategies.

## 12. Future Enhancements
- Add charts to visualize algorithm performance.
- Support loading page-reference sequences from files.
- Add more page replacement algorithms.
- Provide export options for simulation results.