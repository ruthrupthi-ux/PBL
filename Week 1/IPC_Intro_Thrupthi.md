# Introduction to Inter-Process Communication (IPC)

## 1. What is IPC?

IPC (Inter-Process Communication) is a mechanism used by processes to send and receive data or messages while they are running.

It is a mechanism provided by an operating system that allows two or more processes to communicate and exchange data with each other.
A process is a program that is currently running.

Example:
Suppose a music player and a notification service are running at the same time. They may need to communicate to share information. IPC provides methods for this communication.

## 2. Need for IPC:

- Communication: Processes communicate because different processes may perform different parts of a task.
  
- Data sharing: Processes share data because one process may produce data that another process needs.

- Resource sharing: Processes share resources because multiple processes may need to use the same resources.
  
- Coordination: Processes coordinate because their tasks may depend on each other.
  
- synchronisation : Processes synchronize because they must execute in the correct order and avoid conflicts.

# 3. Importance of IPC: 

(1) Improves efficiency by allowing processes to work together.

(2) Supports multitasking by allowing processes to run and communicate concurrently.

(3) Improves resource utilisation through resource sharing.

(4) Helps maintain coordination between processes.

(5) Helps complete complex tasks by dividing work among processes.

## 4. Basic Types of IPC:

The basic types of IPC are:

1. Pipes:
   - Used for communication between processes.
   - Data flows through a pipe from one process
     to another.

2. Message Queues:
   - Allow processes to exchange messages.
   - Messages can be stored until the receiving
     process is ready to receive them.

3. Shared Memory:
   - Allows multiple processes to access a common
     memory area.
   - It is generally faster because data does not
     need to be repeatedly copied.

4. Semaphores:
   - Used mainly for process synchronization.
   - They help control access to shared resources.

5. Sockets:
   - Allow communication between processes.
   - Can be used between processes on the same
     computer or on different computers

# 5. EXAMPLES OF IPC IN REAL SYSTEMS:

* A parent process communicating with a child
  process using a pipe.

* Multiple processes sharing data through
  shared memory.

* A client and server communicating through
  sockets.

* Processes using semaphores to coordinate
  access to shared resources.
