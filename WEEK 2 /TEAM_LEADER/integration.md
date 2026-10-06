# Multi-Process Simulator – Integration Documentation

## 1. Objective

The objective is to integrate the three main
processes of the simulator:

- UI Process
- Core Process
- Logger Process

The integrated system should work as one multi-process simulator
using POSIX Inter-Process Communication (IPC).


## 2. Integration Flow

UI → Core → Logger

## 3. Integration Work

- Integrated the UI, Core and Logger processes.
- Used `launcher.c` to start all three processes.
- Connected the processes using IPC.
- Tested communication between UI and Core.
- Tested logging from Core to Logger.
- Tested the complete simulator.
- Verified that all processes stop correctly.


---

## 4. System Architecture

```text
                    ┌───────────────────────┐
                    │      UI PROCESS       │
                    │                       │
                    │  User Input           │
                    │  Menu                 │
                    │  Result Display       │
                    └───────────┬───────────┘
                                │
                                │ POSIX
                                │ Message Queue
                                ▼
                    ┌───────────────────────┐
                    │     CORE PROCESS      │
                    │                       │
                    │  CPU                  │
                    │  Memory               │
                    │  Stack                │
                    │  Queue                │
                    └───────────┬───────────┘
                                │
                                │ POSIX
                                │ Message Queue
                                ▼
                    ┌───────────────────────┐
                    │    LOGGER PROCESS     │
                    │                       │
                    │  Execution Logs       │
                    │  Error Logs           │
                    └───────────────────────┘




