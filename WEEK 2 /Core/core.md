## Overview
The Core module executes CPU, Memory, Stack and Queue instructions. It communicates
with the UI and Logger processes using anonymous pipes.

## Files
- `Core/core.c` – Core instruction dispatcher and pipe communication.
- `Core/core.h` – Shared Core structures and function declarations.
- `Core/cpu.c` – CPU instructions: LOAD, ADD, SUB, MUL, DIV, PRINT, HALT.
- `Core/memory.c` – Memory instructions: STORE, READ.
- `Core/stack.c` – Stack instructions: PUSH, POP, SPEEK.
- `Core/queue.c` – Queue instructions: ENQUEUE, DEQUEUE, QPEEK.
- `include/ipc_common.h` – Fixed-size message structures shared with UI, Logger and Launcher.
- `testing/test_core_pipes.c` – Small pipe-based integration test used to validate Core.

## Pipe connections
The Launcher must create three pipes:
1. UI → Core: Core reads from `ui_to_core[0]`.
2. Core → UI: Core writes to `core_to_ui[1]`.
3. Core → Logger: Core writes to `core_to_logger[1]`.

In the Core child process, call:
```c
run_core(ui_to_core[0], core_to_ui[1], core_to_logger[1]);
```
The Launcher must close unused pipe ends in each process. UI and Logger must use
the exact message structures in `include/ipc_common.h`. Each message is transferred
as a complete fixed-size C structure.

## Compile the Core object files
Run from the project root:
```bash
gcc -Wall -Wextra -Wpedantic -std=c11 -Iinclude -ICore -c \
    Core/core.c Core/cpu.c Core/memory.c Core/stack.c Core/queue.c
```
This checks compilation; it does not create a standalone executable because
`core.c` intentionally has no `main()` function.

## Run the pipe integration test
From the project root:
```bash
gcc -Wall -Wextra -Wpedantic -std=c11 -I. -Iinclude -ICore \
    testing/test_core_pipes.c Core/core.c Core/cpu.c Core/memory.c \
    Core/stack.c Core/queue.c -o test_core_pipes
./test_core_pipes
```

The test sends the following instructions through anonymous pipes:
```text
LOAD 10
ADD 5
PRINT
STORE 2 100
READ 2
PUSH 25
SPEEK
POP
ENQUEUE 30
QPEEK
DEQUEUE
HALT
```

Expected key results include accumulator 15, memory[2] = 100, stack top 25,
queue front 30, and final CPU message `CPU stopped`.

## Message protocol
- `UIMessage`: UI request, including process ID and instruction.
- `CoreResponse`: Core result sent to UI.
- `LogMessage`: execution log sent to Logger.

All three processes must compile against the same `ipc_common.h`. Do not change
the fields or their order in only one process.

## Notes
- Memory, Stack and Queue each have a capacity of 10.
- `SPEEK` is retained to match the existing project instruction spelling.
- The queue is a simple linear queue, matching the current implementation.
- `HALT` stops the Core instruction loop.
- Full-system execution still depends on the team's Launcher, UI and Logger
  implementing this same pipe and message protocol.
