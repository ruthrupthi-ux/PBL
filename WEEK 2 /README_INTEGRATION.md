# Week 2 Multi-Process Simulator — Integration Notes

## Process architecture

- **UI process:** collects process details and instructions.
- **Core process:** executes CPU, Memory, Stack and Queue instructions.
- **Logger process:** receives Core log records and writes `execution.log` / `error.log`.
- **Launcher:** creates the POSIX message queue and starts the three processes.

## IPC used by this integrated version

- UI ↔ Core: two named FIFOs (`/tmp/ui_to_core_fifo` and `/tmp/core_to_ui_fifo`).
- Core → Logger: one POSIX message queue (`/core_to_log_queue`).

The original source files mixed FIFO, raw file-descriptor writes and POSIX message queues without a matching launcher setup. This version makes the interfaces consistent. It uses the IPC mechanisms already present in the submitted Week 2 files; it does not add shared memory or claim that four IPC mechanisms are required.

## Build and run

From this directory:

```bash
make clean
make
./launcher
```

In the UI:
1. Select `1` and enter process ID, burst time and priority.
2. Select `2` and enter instructions such as `LOAD 10`, `ADD 5`, `PRINT`.
3. Select `5` to exit. Core detects FIFO closure and sends a shutdown record to Logger.

## Standalone test

```bash
printf 'LOAD 10\nADD 5\nPRINT\nHALT\n' | ./core_standalone
```

## Notes

- `Core/core_process.c` is the process entry point (`main`) for the Core executable.
- `Core/core.c` contains the instruction dispatcher and FIFO / message-queue communication loop.
- `Core/core_standalone.c` provides the standalone executable used by the benchmark script.
- The `week 3` folder is not included in this Week 2 package.
