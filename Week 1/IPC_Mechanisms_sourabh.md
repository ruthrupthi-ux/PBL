IPC MECHANISMS

1.SOCKET PROGRAMMING

Socket programming is an IPC mechanism used for communication
between two processes. The processes can be on the same computer
or on different computers connected through a network.

Basic Working:
1. Server creates a socket.
2. Server binds the socket to an address and port.
3. Server waits for a connection.
4. Client creates a socket and connects to the server.
5. Client and server exchange data.
6. Communication is completed and sockets are closed.

Example:
Client sends a message → Server receives it → Server sends a reply.


2.SHARED MEMORY

Shared memory is an IPC mechanism where two or more processes
use the same area of memory to exchange data.

Basic Working:
1. A shared memory area is created.
2. Processes attach to the shared memory.
3. One process writes data into it.
4. Another process reads the data.
5. Processes detach from the shared memory after use.

Example:
Process A writes "Hello" into shared memory.
Process B reads "Hello" from the same memory.


3.COMPARISON

Socket Programming:
- Uses sockets for communication.
- Can communicate over a network.
- Suitable for client-server applications.

Shared Memory:
- Uses a common memory area.
- Mainly used for processes on the same system.
- Provides fast data sharing.

CONCLUSION

Socket programming and shared memory are important IPC mechanisms.
Sockets are useful for communication between systems, while shared
memory is useful for fast communication between processes on the
same system.
