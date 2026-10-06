# IPC Mechanism Selection

## Selected IPC Mechanism

The project uses **FIFO (Named Pipe)** as the POSIX IPC mechanism.

## Why FIFO was selected

FIFO was selected because the simulator consists of separate processes
such as UI, Core and Logger that need to communicate with each other.

The main reasons for selecting FIFO are:

1. **Simple communication**
   - FIFO provides a simple way for one process to send data to another process.

2. **Suitable for separate processes**
   - UI, Core and Logger run as independent processes, so they need an IPC mechanism
     to exchange information.

3. **Easy to implement**
   - FIFO can be created and accessed using standard POSIX system calls.

4. **Useful for logging**
   - The Core process can send execution or error information to the Logger process
     through the FIFO.

5. **Process separation**
   - FIFO allows the processes to remain independent while still communicating.

## Communication Flow

UI → Core → Logger

The UI sends commands to the Core process.
The Core processes the commands and sends relevant execution/error information
to the Logger process through FIFO.

## Conclusion

FIFO was selected because it provides a simple and suitable POSIX IPC mechanism
for communication between the independent processes in our multi-process simulator.
