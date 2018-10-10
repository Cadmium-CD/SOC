#ifndef DO_BLOCK_HAL
#define DO_BLOCK_HAL
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int do_block_hal (int lda, int ldb,int ldc,int M, int N, int K, int* A, int* B, int* C);
void Drv_initialize(void);
void Drv_inputdata1(int data1);
void Drv_inputdata2(int *data2, int height, int width, int ld);
void Drv_startfunction();
int Drv_waitfunction();
void Drv_outputdata(*data, int height, int width, int ld);
void Drv_stopfunction();

#endif
