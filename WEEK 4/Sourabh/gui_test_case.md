# Test Cases — Deadlock Simulation System

## Test 1: Safe State
**Inputs:** 5 processes, 3 resource types; totals `5 3 3`.

Allocation matrix:
```text
0 1 0
2 0 0
0 0 1
1 0 0
0 1 0
```

Maximum matrix:
```text
2 2 2
3 1 1
1 1 2
2 1 1
1 2 1
```

**Expected results**
- Available: `2 1 2`
- Need:
```text
2 1 2
1 1 1
1 1 1
1 1 1
1 1 1
```
- Banker's Safety Check reports a safe state.
- One valid safe sequence: `P0 -> P1 -> P2 -> P3 -> P4`.
- Deadlock detection reports no deadlocked processes.

**Actual result:** Re-run and record the result from the final committed version.

## Test 2: Deadlock Detection
**Inputs:** 2 processes, 2 resource types; totals `1 1`.

Allocation matrix:
```text
1 0
0 1
```

Maximum matrix:
```text
2 1
1 2
```

Submit requests:
| Process | Resource | Amount |
|---|---|---:|
| P0 | R1 | 1 |
| P1 | R0 | 1 |

**Expected results**
- Both requests remain pending because the requested resources are unavailable.
- Detection identifies P0 and P1 as potentially deadlocked.

**Actual result:** Re-run and record the result from the final committed version.

## Test 3: Recovery
Begin with Test 2 after deadlock detection marks P0 and P1 as deadlocked. Click **Recover / Abort Victim** once.

**Expected results**
- One victim is aborted by one recovery action.
- The victim's allocation and request entries are cleared.
- The victim's allocated resources are released.
- The other process is not automatically marked aborted by that action.
- Run detection again and record whether a deadlock remains.

**Actual result:** Re-run and record the result from the final committed version.

## Test 4: Invalid Matrix Input
Try malformed rows, negative values, or Allocation greater than Maximum.
**Expected:** Invalid matrix input is rejected with an error status.
**Actual result:** Record after testing.

## Test 5: Invalid Resource Totals
Set a total below the number of instances currently allocated.
**Expected:** The GUI rejects the totals.
**Actual result:** Record after testing.

## Test 6: Exit
Click **Exit**.
**Expected:** The GUI closes.
**Actual result:** Record after testing.


