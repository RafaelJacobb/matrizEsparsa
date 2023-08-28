#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  float a_values[4][4] = {
      {50, 0, 0, 0}, {10, 0, 20, 0}, {0, 0, 0, 0}, {-30, 0, -60, 5}};
  Matrix *A = matrix_create(4, 4, a_values);
  matrix_print(A);

  float b_values[4][4] = {
      {50, 30, 0, 0}, {10, 0, -20, 0}, {0, 0, 0, 0}, {0, 0, 0, -5}};
  Matrix *B = matrix_create(4, 4, b_values);
  matrix_print(B);

  Matrix *C = matrix_add(A, B);
  matrix_print(C);
  // matrix_destroy(C);
  C = matrix_multiply(A, B);
  matrix_print(C);
  // matrix_destroy(C);
  C = matrix_transpose(A);
  matrix_print(C);
  // matrix_destroy(C);
  // matrix_destroy(A);
  // matrix_destroy(B);
  return 0;
}