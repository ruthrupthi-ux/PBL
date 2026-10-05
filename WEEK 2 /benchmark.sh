#!/bin/bash
# Week 4 benchmark helper.
# Run this script from the WEEK 2 directory after: make
# It compares the same CPU instruction sequence in standalone and multi-process modes.

set -u

echo "========================================"
echo "STANDALONE VS MULTI-PROCESS BENCHMARK"
echo "========================================"
echo
echo "Each mode is run 5 times."
echo "Instruction workload: LOAD 10, ADD 5, PRINT"
echo

echo "--- Standalone Core ---"
for i in 1 2 3 4 5
do
    /usr/bin/time -f "Run $i: real=%e sec, user=%U sec, sys=%S sec" \
        sh -c 'printf "LOAD 10\nADD 5\nPRINT\nHALT\n" | ./core_standalone >/dev/null'
done

echo
echo "--- Multi-Process Simulator ---"
for i in 1 2 3 4 5
do
    /usr/bin/time -f "Run $i: real=%e sec, user=%U sec, sys=%S sec" \
        sh -c 'printf "1\n1\n5\n1\n2\nLOAD 10\n2\nADD 5\n2\nPRINT\n5\n" | ./launcher >/dev/null'
done

echo
echo "Record the average real time for each mode."
echo "IPC overhead can be discussed as the extra time introduced by message passing and process coordination."
echo "For CPU/memory measurements, use:"
echo "  /usr/bin/time -v ./launcher"
echo "and record CPU time and Maximum resident set size."
