#ifndef INCLUDE_MATRIX_H
#define INCLUDE_MATRIX_H

typedef struct {
    int rows;
    int cols;
    int **data;  // arrays of arrays are used. So data[1] will point to another array ar[n].
} Matrix;

Matrix *create_matrix(int rows, int cols);
int mget(Matrix *matrix, int row, int col);
void mset(Matrix *matrix, int row, int col, int value);
void mfree(Matrix *matrix);
int *mflat(Matrix *matrix);
char *mflatRGB(Matrix *matrix, int nComps);

#endif