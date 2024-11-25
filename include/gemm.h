#pragma once

void gemm(int TA, int TB, int M, int N, int K, float ALPHA, float *A, int lda,
          float *B, int ldb, float BETA, float *C, int ldc);
void gemm_bin(int M, int N, int K, float ALPHA, char *A, int lda, float *B,
              int ldb, float *C, int ldc);
float *random_matrix(int rows, int cols);
void time_random_matrix(int TA, int TB, int m, int k, int n);
void gemm_nn(int M, int N, int K, float ALPHA, float *A, int lda, float *B,
             int ldb, float *C, int ldc);
void gemm_nt(int M, int N, int K, float ALPHA, float *A, int lda, float *B,
             int ldb, float *C, int ldc);
void gemm_tn(int M, int N, int K, float ALPHA, float *A, int lda, float *B,
             int ldb, float *C, int ldc);
void gemm_tt(int M, int N, int K, float ALPHA, float *A, int lda, float *B,
             int ldb, float *C, int ldc);
