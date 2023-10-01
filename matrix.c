#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "matrix.h"

void println(const char *format, ...)
{
       va_list args;
       vprintf(format, args);
       printf("\n");
       va_end(args);
}

// Function to create a new matrix
Matrix *create_matrix(int rows, int cols)
{
       Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
       if (matrix == NULL)
       {
              println("Failed to allocate memory for the matrix");
              exit(EXIT_FAILURE);
       }

       matrix->rows = rows;
       matrix->cols = cols;

       // Allocate memory for the 2D array
       matrix->data = (int **)malloc(rows * sizeof(int *));
       if (matrix->data == NULL)
       {
              println("Failed to allocate memory for the matrix data");
              exit(EXIT_FAILURE);
       }

       for (int i = 0; i < rows; i++)
       {
              matrix->data[i] = (int *)malloc(cols * sizeof(int));
              if (matrix->data[i] == NULL)
              {
                     println("Failed to allocate memory for the matrix row");
                     exit(EXIT_FAILURE);
              }
       }

       return matrix;
}

// Function to read an element of the matrix
int mget(Matrix *matrix, int row, int col)
{
       if (row >= 0 && row < matrix->rows && col >= 0 && col < matrix->cols)
       {
              return matrix->data[row][col];
       }
       else
       {
              return 0;
       }
}

// Function to write an element to the matrix
void mset(Matrix *matrix, int row, int col, int value)
{
       if (row >= 0 && row < matrix->rows && col >= 0 && col < matrix->cols)
       {
              matrix->data[row][col] = value;
       }
       else
       {
              fprintf(stderr, "Invalid matrix element access at row %d, col %d\n", row, col);
              exit(EXIT_FAILURE);
       }
}

// Function to free the memory allocated for the matrix
void mfree(Matrix *matrix)
{
       for (int i = 0; i < matrix->rows; i++)
       {
              free(matrix->data[i]);
       }
       free(matrix->data);
       free(matrix);
}

int *mflat(Matrix *matrix)
{
       int *flatmap = (int *)malloc(sizeof(int) * matrix->cols * matrix->rows);
       for (int i = 0; i < matrix->rows; i++)
       {
              for (int j = 0; j < matrix->cols; j++)
              {
                     flatmap[i * matrix->cols + j] = mget(matrix, i, j);
              }
       }
       return flatmap;
}

char *mflatRGB(Matrix *matrix, int nComps)
{
       int start=0;
       char *flatmap = (char *)malloc(sizeof(char) * matrix->cols * matrix->rows * nComps);
       printf("%d width %d height %d total chars\n", matrix->cols, matrix->rows, matrix->cols * matrix->rows * nComps);
       for (int i = 0; i < matrix->rows; i++)
       {
              for (int j = 0; j < matrix->cols; j++)
              {
                     int rgb = mget(matrix, i, j);
                     char r = (rgb & 0x00FF0000) >> 16;
                     char g = (rgb & 0x0000FF00) >> 8;
                     char b = rgb & 0x000000FF;
                     start = i * matrix->cols*3 + j * 3;
                     flatmap[start] = r;
                     flatmap[start + 1] = g;
                     flatmap[start + 2] = b;
              }
       }
       printf("start %d", start);
       return flatmap;
}