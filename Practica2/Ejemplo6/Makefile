CC=icc
CFLAGS=-Wall -std=c99 -O3 -qopenmp -qopt-report=3

LDFLAGS=-lm

all: heat

heat: heat.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -rf *.o heat
