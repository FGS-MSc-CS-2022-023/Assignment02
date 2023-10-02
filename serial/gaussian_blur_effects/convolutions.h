#ifndef CONVOLUTIONS_H
#define CONVOLUTIONS_H

#include "matrix.h"

int *create_gaussian_kernel(int size, int sigma, int *kernel_total);
void convolution_gaussian_blur(Matrix *image, int kernelsize);
void convolution_sharpen(Matrix *image);
void convolution(Matrix *image, int *kernel, int size, int kernel_value);
void convolution_infrared(Matrix *image);
void grayscale(Matrix *image);

#endif