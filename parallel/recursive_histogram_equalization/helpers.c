#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "matrix.h"

void dumpArrayChar(char *ar, int len, char *path)
{
    FILE *file = fopen(path, "w");

    for (int i = 0; i < len; i++)
    {
        fprintf(file, "%d ", ar[i]);
    }
    fclose(file);
}

void dumpArrayInt(int *ar, int len, char *path)
{
    FILE *file = fopen(path, "w");

    for (int i = 0; i < len; i++)
    {
        fprintf(file, "%d ", ar[i]);
    }
    fclose(file);
}

void dumpArrayDouble(double *ar, int len, char *path)
{
    FILE *file = fopen(path, "w");

    for (int i = 0; i < len; i++)
    {
        fprintf(file, "%f ", ar[i]);
    }
    fclose(file);
}

void eqArray(char *ar1, char *ar2, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (ar1[i] != ar2[i])
        {
            printf("Not equal at %d, total:%d, %d, %d \n", i, len, ar1[i], ar2[i]);
        }
    }

    printf("Equal\n");
}

Matrix *load_image(char *path)
{
    int width, height, comp;
    unsigned char *data = stbi_load(path, &width, &height, &comp, 0);
    if (data)
    {
        printf("width = %d, height = %d, comp = %d (channels)\n", width, height, comp);
        Matrix *matrix = create_matrix(height, width);

        #pragma omp parallel shared(data)
        for (int i = 0; i < height; i++)
        {
            #pragma omp parallel for
            for (int j = 0; j < width; j++)
            {
                int r, g, b;
                int startbyte = i * width * 3 + j * 3;
                r = data[startbyte];
                g = data[startbyte + 1];
                b = data[startbyte + 2];
                int rgb = b + (g << 8) + (r << 16);
                mset(matrix, i, j, rgb);
            }
        }

        // for (int i = 0; i < 1; i++)
        // {
        //     for (int j = 0; j < width; j++)
        //     {
        //         printf("row:%d col:%d is %08x\n", i, j, mget(matrix, i, j));
        //     }
        // }

        // char *flatmap = mflatRGB(matrix, 3);
        //  eqArray(data, flatmap, width * height * 3);

        // stbi_write_jpg("./ar.jpg", width, height, 3, flatmap, 80);

        // printf("\n");
        return matrix;
    }
    return NULL;
}

void save_image(char *path, Matrix *image)
{
    char *flatmap = mflatRGB(image, 3);
    int result = stbi_write_jpg(path, image->cols, image->rows, 3, flatmap, 80);
    printf("test %s %d\n", path, result);
}