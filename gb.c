#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "matrix.h"

void dumpArray(char *ar, int len, char *path)
{
    FILE *file = fopen(path, "w");

    for (int i = 0; i < len; i++)
    {
        fprintf(file, "%d ", ar[i]);
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

Matrix *LoadImage(char *path)
{
    int width, height, comp;
    unsigned char *data = stbi_load(path, &width, &height, &comp, 0);
    if (data)
    {
        printf("width = %d, height = %d, comp = %d (channels)\n", width, height, comp);
        Matrix *matrix = create_matrix(height, width);

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
    stbi_write_jpg(path, image->cols, image->rows, 3, flatmap, 80);
}

void gausian_blur(Matrix *image)
{
    //int kernel[9] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
    int kernel[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
    for (int i = 0; i < image->rows; i++)
    {
        for (int j = 0; j < image->cols; j++)
        {
            int targetVal = mget(image, i, j), gausian_g = 0, gausian_b = 0, gausian_r = 0;
            int imgMatrix[9], kernel_val = 0;

            for (int k = 0; k < 9; k++)
            {
                kernel_val += kernel[k];
                imgMatrix[k] = mget(image, i - 1 + k / 3, j - 1 + k % 3);
                gausian_b += (imgMatrix[k] & 0xFF) * kernel[k];
                gausian_g += ((imgMatrix[k] >> 8) & 0xFF) * kernel[k];
                gausian_r += ((imgMatrix[k] >> 16) & 0xFF) * kernel[k];
            }

             gausian_b /= kernel_val;
             gausian_g /= kernel_val;
             gausian_r /= kernel_val;

            //printf("%d %d %d\n", gausian_r, gausian_g, gausian_b);

            targetVal = gausian_b + (gausian_g << 8) + (gausian_r << 16);
            mset(image, i, j, targetVal);
        }
    }
    printf("loop ok");
}


void bnw(Matrix *image)
{
    for (int i = 0; i < image->rows; i++)
    {
        for (int j = 0; j < image->cols; j++)
        {           

            int targetValue = mget(image, i, j);
            int r = targetValue & 0xFF00 >> 8; //+ ((targetValue & 0xFF00) >> 8) + ((targetValue & 0xFF0000) >> 16);
           
            targetValue = r + (r << 8) + (r << 16);
           
            mset(image, i, j, targetValue);
        }
    }
}

int main()
{
    Matrix *mImage = LoadImage("./images/img2.jpg");
    //save_image("./output/ar.jpg", mImage);
    //gausian_blur(mImage);
    bnw(mImage);
    gausian_blur(mImage);
    save_image("./output/img23.jpg", mImage);
}