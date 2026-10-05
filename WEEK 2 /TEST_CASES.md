# IPC Test Cases – Week 2

| ID | Test case | Input / action | Expected result |
|---|---|---|---|
| TC01 | Start system | `./launcher` | UI, Core and Logger start as separate processes |
| TC02 | Create process | ID=5, Burst=1, Priority=1 | UI displays captured process |
| TC03 | UI → Core IPC | `LOAD 10` | Core receives and executes instruction |
| TC04 | Core → UI IPC | Execute any valid instruction | UI receives completion response |
| TC05 | Core → Logger IPC | Execute `ADD 5` | Logger receives event and writes log |
| TC06 | CPU instruction | `LOAD 10`, `ADD 5`, `PRINT` | Accumulator reaches 15 |
| TC07 | Memory instruction | STORE 0 25, READ 0 | Memory location 0 stores/reads 25 |
| TC08 | Stack instruction | PUSH 50, POP | Stack operations work |
| TC09 | Queue instruction | ENQUEUE 100, DEQUEUE | Queue operations work |
| TC10 | Invalid instruction | `XYZ 10` | UI receives error and Logger records ERROR |
| TC11 | Divide by zero | `DIV 0` | Existing CPU reports divide-by-zero |
| TC12 | Shutdown | Choose Exit | Processes stop and queues are cleaned |
| TC13 | Log file | Open `logs/simulator.log` | Timestamped execution events are present |
