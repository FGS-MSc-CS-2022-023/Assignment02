#ifndef INCLUDE_HELPERS_H
#define INCLUDE_HELPERS_H
#include "matrix.h"

void save_image(char *path, Matrix *image);
Matrix *load_image(char *path);
void eqArray(char *ar1, char *ar2, int len);
void dumpArrayChar(char *ar, int len, char *path);
void dumpArrayInt(int *ar, int len, char *path);
void dumpArrayDouble(double *ar, int len, char *path);

#endif