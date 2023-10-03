#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <dirent.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include "matrix.h"
#include "omp.h"
#include "helpers.h"

char **get_files(char *dirname, int *count)
{
    DIR *dir;
    struct dirent *dirent;
    char **names = NULL;
    struct stat stat1;
    int i = 0;

    if ((dir = opendir(dirname)) != NULL)
    {
        names = (char **)malloc(sizeof(char *) * 10);
        while (((dirent = readdir(dir)) != NULL) && i < 10)
        {
            char filename[260];
            sprintf(filename, "%s/%s", dirname, dirent->d_name);
            stat(filename, &stat1);
            if (S_ISREG(stat1.st_mode))
            {
                char *name = (char*)malloc(sizeof(char) * dirent->d_namlen);
                strcpy(name, dirent->d_name);
                names[i] = name;
                i++;
            }
        }
    }
    *count = i;
    return names;
}

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
            println("Not equal at %d, total:%d, %d, %d \n", i, len, ar1[i], ar2[i]);
        }
    }

    println("Equal\n");
}

Matrix *load_image(char *path)
{
    int width, height, comp;
    unsigned char *data = stbi_load(path, &width, &height, &comp, 0);
    if (data)
    {
        println("width = %d, height = %d, comp = %d (channels)\n", width, height, comp);
        Matrix *matrix = create_matrix(height, width);

#pragma omp parallel for collapse(2) shared(data, matrix) schedule(guided)
        for (int i = 0; i < height; i++)
        {
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
        //         println("row:%d col:%d is %08x\n", i, j, mget(matrix, i, j));
        //     }
        // }

        // char *flatmap = mflatRGB(matrix, 3);
        //  eqArray(data, flatmap, width * height * 3);

        // stbi_write_jpg("./ar.jpg", width, height, 3, flatmap, 80);

        // println("\n");
        return matrix;
    }
    return NULL;
}

void save_image(char *path, Matrix *image)
{
    char *flatmap = mflatRGB(image, 3);
    int result = stbi_write_jpg(path, image->cols, image->rows, 3, flatmap, 80);
    println("test %s %d\n", path, result);
}