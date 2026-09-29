Introduction to Inter-Process Communication (IPC)

 1. What is IPC?

IPC (Inter-Process Communication) is a mechanism used by processes to send and receive data or messages while they are running.

It is a mechanism provided by an operating system that allows two or more processes to communicate and exchange data with each other.
A process is a program that is currently running.

Example:
Suppose a music player and a notification service are running at the same time. They may need to communicate to share information. IPC provides methods for this communication.

2. Need for IPC

- Communication: Processes communicate because different processes may perform different parts of a task.
  
- Data sharing: Processes share data because one process may produce data that another process needs.

- Resource sharing: Processes share resources because multiple processes may need to use the same resources.
  
- Coordination: Processes coordinate because their tasks may depend on each other.
  
- synchronisation : Processes synchronize because they must execute in the correct order and avoid conflicts.

3. IMPORTANCE OF IPC 

(i) Improves efficiency by allowing processes to work together.
(ii) Supports multitasking by allowing processes to run and communicate concurrently.
(iii) Improves resource utilisation through resource sharing.
(iv) Helps maintain coordination between processes.
(v) Helps complete complex tasks by dividing work among processes.

## 4. Basic Types of IPC

The basic types of IPC are:

- **Pipes:** Used to send data between processes, commonly between related processes.
- **Message Queues:** Allow processes to exchange messages through a queue.
- **Shared Memory:** Allows multiple processes to access a common area of memory.
- **Sockets:** Allow processes to communicate, especially between processes on different systems.
- **Signals:** Used to notify a process that a particular event has occurred.
