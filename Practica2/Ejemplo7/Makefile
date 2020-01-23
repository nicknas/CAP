CC=icl
CFLAGS=-O3 -Wall -qopenmp -qopt-report=3 -xhost
LDFLAGS=

TARGETS=demo headless
SOURCES=$(shell echo *.c)
COMMON_OBJECTS=timing.o solver.o

all: $(TARGETS)

demo: demo.o $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lGL -lGLU -lglut

headless: headless.o $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o .depend *~ *.optrpt

.depend: *.[ch]
	$(CC) -MM $(SOURCES) >.depend

-include .depend

.PHONY: clean all
