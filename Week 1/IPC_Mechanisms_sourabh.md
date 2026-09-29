# **IPC MECHANISMS**

### **1. Socket Programming**

Socket programming is an IPC mechanism used for communication between two processes. The processes can be on the same computer or on different computers through a network.

#### **Basic Working**
1. Server creates a socket.
2. Server binds the socket to an address and port.
3. Server waits for a connection.
4. Client creates a socket and connects to the server.
5. Client and server exchange data.
6. Sockets are closed after communication.

**Example:**  
Client sends a message → Server receives it → Server sends a reply.

---

### **2. Shared Memory**

Shared memory is an IPC mechanism where two or more processes use the same area of memory to exchange data.

#### **Basic Working**
1. A shared memory area is created.
2. Processes attach to the shared memory.
3. One process writes data.
4. Another process reads the data.
5. Processes detach from the shared memory.

**Example:**  
Process A writes **"Hello"** → Process B reads **"Hello"**.

---

### **3. Comparison**

| **Socket Programming** | **Shared Memory** |
|---|---|
| Uses sockets for communication | Uses a common memory area |
| Can work over a network | Mainly used on the same system |
| Suitable for client-server communication | Suitable for fast data sharing |

### **Conclusion**

**Socket Programming** and **Shared Memory** are important **IPC mechanisms**. Sockets are useful for communication over networks, while shared memory provides fast communication between processes.
