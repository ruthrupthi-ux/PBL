 # IPC Applications & Advantages
 ## Inter-Process Communication

## 1. Applications of IPC
- IPC is used when two or more processes need to exchange data or send information to each other.
- It is commonly used in operating systems to make different processes work together.
- In client-server programs, IPC helps the client process communicate with the server process.
- It is useful in applications where one process produces data and another process uses it.
- IPC can also be used in multitasking systems where processes need to coordinate their work.

## 2. Advantages of IPC
- It allows processes to share information easily.
- It helps different processes work together and improves coordination.
- IPC can make programs faster by allowing processes to perform tasks at the same time.
- It supports better use of system resources.
- It is useful for building large applications by dividing the work into separate processes.

## 3. Limitations
- IPC mechanisms can make a program more complicated to design and understand.
- Processes may face synchronization problems if communication is not handled properly.
- Shared resources can cause conflicts when multiple processes try to access them at the same time.
- Some IPC methods may have extra overhead because data has to be transferred between processes.
- Incorrect use of IPC can lead to problems such as deadlock or data inconsistency.

## 4. Short Comparison of IPC Mechanisms
- **Pipe:** Transfers data between processes. It is simple and useful for related processes.
  
**Example:** Parent–child communication.
- **Message Queue:** Processes send and receive messages. It is useful when messages are needed.
  
**Example:** Sending commands/data.
- **Shared Memory:** Processes use a common memory area. It is very fast for large data.

**Example:** Sharing data between processes.
- **Socket:** Communication takes place through an endpoint. It is useful for processes on the same or different systems.
  
**Example:** Client-server communication.
