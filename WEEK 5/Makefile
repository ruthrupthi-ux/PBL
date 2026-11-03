
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
GTK_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-3.0)

INCLUDES = -Iinclude -Iananya -IRanvil -ISourabh
SOURCES = Thrupthi/main.c \
          Thrupthi/gui.c \
          ananya/fifo.c \
          Ranvil/lru.c \
          Sourabh/optimal.c

TARGET = page_replacement_simulator

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SOURCES) include/page_replacement.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET) $(GTK_LIBS)

test:
	$(CC) $(CFLAGS) $(INCLUDES) ananya/fifo.c ananya/test_fifo.c -o ananya/test_fifo
	./ananya/test_fifo
	$(CC) $(CFLAGS) $(INCLUDES) Ranvil/lru.c Ranvil/test_lru.c -o Ranvil/test_lru
	./Ranvil/test_lru
	$(CC) $(CFLAGS) $(INCLUDES) Sourabh/optimal.c Sourabh/test_optimal.c -o Sourabh/test_optimal
	./Sourabh/test_optimal

clean:
	rm -f $(TARGET) ananya/test_fifo Ranvil/test_lru Sourabh/test_optimal
	rm -f *.o
