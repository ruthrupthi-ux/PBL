# IPC Mechanism Selection

## Selected IPC Mechanism

The project uses **POSIX Message Queues** as the IPC mechanism.

## Why POSIX Message Queues were selected

POSIX Message Queues were selected because the simulator
consists of separate UI, Core and Logger processes that need
to communicate with each other.

The main reasons are:

1. **Simple communication**
   - Processes can send and receive messages easily.

2. **Suitable for separate processes**
   - UI, Core and Logger run as independent processes.

3. **Structured messages**
   - Commands, responses and log messages can be sent
     in a structured format.

4. **Reliable IPC**
   - POSIX Message Queues provide a suitable mechanism
     for communication between the processes.

## Queues Used

- `UI_TO_CORE_QUEUE` – UI sends commands to Core.
- `CORE_TO_UI_QUEUE` – Core sends responses to UI.
- `CORE_TO_LOG_QUEUE` – Core sends log messages to Logger.

## Result

IPC communication between UI, Core and Logger was
tested successfully using POSIX Message Queues.
