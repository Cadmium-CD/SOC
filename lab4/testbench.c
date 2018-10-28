#include "gemm_lab4.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	int lda = 2;
	int ldb = 4;
	int ldc = 4;
	int M = 3;
	int N = ldb;
	int K = lda;
	int *A = malloc(M*lda*sizeof(int));
	int *B = malloc(ldb*lda*sizeof(int));
	int *C = calloc(M*ldc,sizeof(int));

	int i;

	for (i = 0; i < M * lda; ++i)
		A[i] = i;
	for (i = 0; i < lda * ldb; ++i)
		B[i] = i*2;

	gemm_lab4(lda,ldb,ldc,M,N,K,A,B,C);
	for (i = 0; i < M * ldc; ++i)
	{
		printf("%d\t", C[i]);
	}

	return 0;
} 