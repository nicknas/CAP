


CC=gcc

CFLAGS=-O3 -lrt -std=c99

LIB=-lm

PGCC=pgcc -Minfo -fast -acc -ta=nvidia -tp=nehalem

CFLAGS_OFFLOAD = $(CFLAGS) -fopenmp -fno-stack-protector

NAME=jacobi

default: host gpu

host:
	$(CC) $(CFLAGS) $(NAME).c -o $(NAME).host.exe $(LIB)

gpu: pgi omp_offload

pgi:
	$(PGCC) $(NAME).c -o $(NAME).pgi.exe $(LIB)

omp_offload:
	$(CC) $(CFLAGS_OFFLOAD) $(NAME).c -o $(NAME).gccoffload.exe $(LIB)

clean: 
	rm -f __hmpp_*  *.exe
