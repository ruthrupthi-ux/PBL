# Multi-Process Simulator – Integration Documentation

## 1. Objective

The objective is to integrate the three main
processes of the simulator:

- UI Process
- Core Process
- Logger Process

The integrated system should work as one multi-process simulator
using POSIX Inter-Process Communication (IPC).

---

## 2. System Architecture

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




