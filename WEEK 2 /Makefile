CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lrt

all: UI core_process logger launcher core_standalone

UI: UI.c ipc_common.h
	$(CC) $(CFLAGS) UI.c -o UI $(LDFLAGS)

core_process: Core/core.c Core/cpu.c Core/memory.c Core/stack.c Core/queue.c ipc_common.h
	$(CC) $(CFLAGS) -I. Core/core.c -o core_process $(LDFLAGS)

logger: logger.c ipc_common.h
	$(CC) $(CFLAGS) logger.c -o logger $(LDFLAGS)

launcher: launcher.c ipc_common.h
	$(CC) $(CFLAGS) launcher.c -o launcher $(LDFLAGS)

core_standalone: Core/core_standalone.c Core/cpu.c Core/memory.c Core/stack.c Core/queue.c
	$(CC) $(CFLAGS) Core/core_standalone.c -o core_standalone

clean:
	rm -f UI core_process logger launcher core_standalone
	rm -rf logs

run: all
	./launcher 