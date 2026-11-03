CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -O2
LDFLAGS = -lrt

CORE_SOURCES = Core/core.c Core/cpu.c Core/memory.c Core/stack.c Core/queue.c

.PHONY: all clean run
all: UI core_process logger launcher core_standalone

UI: UI.c ipc_common.h
	$(CC) $(CFLAGS) UI.c -o UI $(LDFLAGS)

core_process: Core/core_process.c $(CORE_SOURCES) Core/core.h ipc_common.h
	$(CC) $(CFLAGS) -I. -ICore Core/core_process.c $(CORE_SOURCES) -o core_process $(LDFLAGS)

logger: logger.c ipc_common.h
	$(CC) $(CFLAGS) logger.c -o logger $(LDFLAGS)

launcher: launcher.c ipc_common.h
	$(CC) $(CFLAGS) launcher.c -o launcher $(LDFLAGS)

core_standalone: Core/core_standalone.c Core/cpu.c Core/memory.c Core/stack.c Core/queue.c Core/core.h
	$(CC) $(CFLAGS) -ICore Core/core_standalone.c Core/cpu.c Core/memory.c Core/stack.c Core/queue.c -o core_standalone

clean:
	rm -f UI core_process logger launcher core_standalone
	rm -f execution.log error.log
	rm -f /tmp/ui_to_core_fifo /tmp/core_to_ui_fifo
	-rm -f *.o

run: all
	./launcher
