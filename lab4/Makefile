CC=gcc
CFLAGS=-I.
DEPS = src/gemm_lab4.h
OBJ = src/gemm_lab4.o src/testbench.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

testbench: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
