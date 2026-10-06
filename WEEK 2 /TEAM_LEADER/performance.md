# Performance Analysis

## Objective

To compare the standalone simulator and multi-process simulator.

## Test

The same instructions were used:

- LOAD 10
- ADD 5
- PRINT
- HALT

Each program was tested 5 times.

## Results

| Test | Standalone | Multi-Process |
|------|------------|---------------|
| Run 1 | 0.00 sec | 0.00 sec |
| Run 2 | 0.01 sec | 0.00 sec |
| Run 3 | 0.00 sec | 0.00 sec |
| Run 4 | 0.00 sec | 0.00 sec |
| Run 5 | 0.00 sec | 0.00 sec |

## CPU Usage

- User time: 0.00 sec
- System time: 0.00 sec

## Memory Usage

- Maximum memory used: 1892 KB

## IPC Overhead

The multi-process simulator has some extra overhead because
the UI, Core and Logger processes communicate using IPC.

## Conclusion

The standalone simulator is simpler and has less overhead.

The multi-process simulator uses more resources, but it provides
separate UI, Core and Logger processes and demonstrates IPC.
