#pragma once

typedef struct matrix {
  int rows, cols;
  float **vals;
} matrix;

matrix copy_matrix(matrix m);
void print_matrix(matrix m);

matrix hold_out_matrix(matrix *m, int n);
matrix resize_matrix(matrix m, int size);

float *pop_column(matrix *m, int c);