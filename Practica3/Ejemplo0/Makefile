
CC=gcc

CFLAGS=-O3 -lm -lrt -std=c99

PGCC=pgcc -Minfo -fast -acc -ta=nvidia -tp=nehalem

CFLAGS_OFFLOAD = $(CFLAGS) -fopenmp -fno-stack-protector


NAME=hello

default: host gpu

host:
	$(CC) $(CFLAGS) $(NAME).c -o $(NAME).host.exe

gpu: pgi omp_offload

pgi:
	$(PGCC) $(NAME).c -o $(NAME).pgi.exe

omp_offload:
	$(CC) $(CFLAGS_OFFLOAD) $(NAME).c -o $(NAME).gccoffload.exe

clean: 
	rm -f __hmpp_*  *.exe
