# Test Results

## Objective

To check whether the UI, Core and Logger processes work correctly.

## Test Cases

| Test | Input / Action | Result | Status |
|---|---|---|---|
| 1 | Create Process | Process created successfully | PASS |
| 2 | LOAD 10 | Accumulator = 10 | PASS |
| 3 | ADD 5 | Accumulator = 15 | PASS |
| 4 | PRINT | Result displayed correctly | PASS |
| 5 | View Process Status | Process status displayed | PASS |
| 6 | View Execution Result | Execution result displayed | PASS |
| 7 | Exit | All processes stopped and IPC cleaned | PASS |
| 8 | Logger | Execution messages saved to execution.log | PASS |

## Logger Test

The Logger process successfully received messages from the Core process.

Example:

`Instruction executed: ADD 5`

The message was saved in `execution.log`.

## Final Result

All tested functions worked successfully.

The UI, Core and Logger processes communicated correctly,
and the simulator stopped successfully after the Exit option.
