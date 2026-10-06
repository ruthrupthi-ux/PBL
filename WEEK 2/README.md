# Student 3 - Logging Process

## Responsibility
Student 3 is responsible for the **Logging Process**.

The Logging Process:
- Receives messages from another process.
- Records normal execution messages.
- Records error messages separately.
- Adds date and time to every log entry.
- Uses POSIX FIFO for inter-process communication.

## Files

- `logger.c` - Main Logging Process.
- `sender_test.c` - Small test program that acts like the Core Process.
- `execution.log` - Created automatically for normal execution messages.
- `error.log` - Created automatically for error messages.

## IPC Mechanism

A **POSIX Named Pipe (FIFO)** is used.

The Core Process sends a message through:

`simulator_log_fifo`

The Logging Process reads the message and stores it in the appropriate log file.

## Compile

```bash
gcc logger.c -o logger
gcc sender_test.c -o sender_test
```

## Run

Open Terminal 1:

```bash
./logger
```

Open Terminal 2:

```bash
./sender_test
```

The logger will receive the messages and create:

```text
execution.log
error.log
```

## Example

Normal message:

```text
Execution: CPU executed instruction ADD.
```

is stored in `execution.log`.

Error message:

```text
ERROR: Invalid instruction encountered.
```

is stored in `error.log`.

## Student 3 Contribution

Implemented the Logging Process for the multi-process simulator. The module receives execution and error messages through POSIX FIFO IPC and stores them in separate log files with timestamps.
