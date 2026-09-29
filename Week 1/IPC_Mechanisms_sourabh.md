# **IPC MECHANISMS**


IPC provides different mechanisms for processes to communicate.
Each mechanism works differently depending on how data needs to
be shared, transferred, or synchronized.

### **Main IPC Mechanisms**

- **Pipes**
- **Named Pipes (FIFO)**
- **Message Queues**
- **Shared Memory**
- **Semaphores**
- **Signals**
- **Sockets**

---

## **1. Pipes**

- A pipe provides a communication channel between processes.
- One process writes data and another process reads the data.
- It is mainly used for communication between related processes.

## **2. Named Pipes (FIFO)**

- A Named Pipe is a pipe that has a specific name.
- It allows unrelated processes to communicate with each other.
- Data is read in the same order in which it is written.

## **3. Message Queues**

- A message queue stores messages sent by processes.
- A receiving process can read the messages from the queue.
- It is useful when processes need to exchange separate messages.

## **4. Shared Memory**

- Shared memory allows processes to access a common memory area.
- One process can write data while another process can read it.
- It provides fast communication between processes.

## **5. Semaphores**

- A semaphore is used to control access to shared resources.
- It helps prevent multiple processes from accessing a resource incorrectly.
- It is mainly used for process synchronization.

## **6. Signals**

- A signal is a notification sent to a process.
- It informs a process that a particular event has occurred.
- It is mainly used for process control and notifications.

## **7. Sockets**

- A socket provides an endpoint for communication between processes.
- It can be used on the same computer or between different computers.
- It is commonly used for client-server and network communication.

---

## **Simple Comparison**

| **Mechanism** | **Main Function** |
|---|---|
| **Pipe** | Data communication |
| **Named Pipe** | Unrelated process communication |
| **Message Queue** | Message exchange |
| **Shared Memory** | Fast data sharing |
| **Semaphore** | Synchronization |
| **Signal** | Notification |
| **Socket** | Local and network communication |


