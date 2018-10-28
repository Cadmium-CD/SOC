#include "gemm_lab4.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#if defined(_OPENMP)
#include <omp.h>
#endif

void gemm_lab4 (int lda,int ldb, int ldc, int M, int N, int K, int A[2700], int B[27000], int C[100000])
{
	int i,j,k;

	for (i = 0; i < M; ++i) {
        for (k = 0; k < K; ++k) {
    		int A_PART = A[i*lda + k];
            for (j = 0; j < N; ++j) {
				C[i*ldc + j] += A_PART * B[k*ldb + j];
			}
		}
	}
}

