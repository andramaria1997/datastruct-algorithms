CC = gcc
CFLAGS = -Wall
LDFLAGS = -L.

all: build

build: priorityQ.o
	$(CC) $(CFLAGS) $^ -o priorityQ -lcompare $(LDFLAGS)

priorityQ.o: priorityQ.c
	$(CC) -c priorityQ.c -o $@

clean:
	rm -f *.o priorityQ

