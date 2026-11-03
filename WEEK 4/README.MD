# Interactive Deadlock Simulation System

## 1. Project Overview

The Interactive Deadlock Simulation System is a C-based application with a graphical user interface (GUI) for simulating resource allocation, checking system safety, detecting deadlocks, and recovering from deadlocked states.

The project demonstrates operating system concepts related to process management, resource allocation, deadlock detection, and recovery.

## 2. Objectives

- Simulate processes and resource allocation.
- Maintain Allocation, Maximum, Need, Request, and Available resource information.
- Check system safety using the Banker's Algorithm.
- Detect deadlocked processes.
- Recover from deadlock by terminating a selected deadlocked process and releasing its resources.
- Visualize process-resource relationships using a Resource Allocation Graph.

## 3. Technologies Used

- Programming Language: C
- GUI: GTK 3
- Compiler: GCC
- Development Environment: Visual Studio Code with Ubuntu/WSL
- Version Control: Git and GitHub

## 4. Project Structure

- `Ananya/` – Banker's Algorithm module
- `Include/` – Shared definitions and common header files
- `Ranvil/` – Deadlock detection and recovery modules
- `Sourabh/` – GUI and Resource Allocation Graph modules
- `Thrupthi/` – Main program and resource management module

## 5. Features

- Create and reset the simulation system.
- Configure the number of processes and resource types.
- Edit resource allocation and maximum-demand matrices.
- Set total resource instances.
- Check whether the system is in a safe state and display a safe sequence.
- Submit resource requests and record pending requests when resources are unavailable.
- Detect deadlocked processes.
- Recover by aborting a deadlocked process and releasing its allocated resources.
- Display resource allocation and request edges graphically.

## 6. Requirements

- GCC compiler
- GTK 3 development libraries
- `pkg-config`
- Linux or Ubuntu/WSL environment

## 7. Compilation

Run the following command from the project directory containing the `Thrupthi`, `Ananya `, `Ranvil`, and `Sourabh` folders:

```bash
gcc -Wall -Wextra -std=c11 \
  Thrupthi/main.c \
  Thrupthi/resource_manager.c \
  'Ananya /banker.c' \
  Ranvil/deadlock.c \
  Ranvil/recovery.c \
  Sourabh/gui.c \
  Sourabh/rag.c \
  -o deadlock_simulator \
  $(pkg-config --cflags --libs gtk+-3.0) -lm
```

## 8. Running the Application

After successful compilation, run:

```bash
./deadlock_simulator
```

## 9. Testing

The following scenarios were tested during development:

1. Banker’s Safety Check displaying a safe sequence.
2. Recording a resource request when no instances are available.
3. Detecting deadlocked processes P0 and P1 in a circular-wait scenario.
4. Recovering from deadlock by aborting P0 and releasing its allocated resources.
5. Checking the system safety again after recovery.

## 10. Expected Outcome

The application provides a graphical demonstration of resource allocation, safe-state checking, deadlock detection, and deadlock recovery.

## 11. Conclusion

The project demonstrates key operating system concepts through an interactive simulation. It helps users understand how resource allocation can lead to deadlocks and how detection and recovery techniques can restore system progress.

