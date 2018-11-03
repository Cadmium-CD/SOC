#include "gemm_lab4.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	int lda = 20;
	int ldb = 1000;
	int ldc = 1000;
	int M = 100;
	int N = ldb;
	int K = lda;
	int *A = malloc(M*lda*sizeof(int));
	int *B = malloc(ldb*lda*sizeof(int));
	int *C = calloc(M*ldc,sizeof(int));

	int i;

	for (i = 0; i < M * lda; ++i)
		A[i] = i;
	for (i = 0; i < lda * ldb; ++i)
		B[i] = 1;

	gemm_lab4(lda,ldb,ldc,M,N,K,A,B,C);
	for (i = 0; i < M * ldc; ++i)
	{
		//printf("%d\t", C[i]);
	}

	return 0;
} 