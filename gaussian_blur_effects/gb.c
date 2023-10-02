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
    int result = stbi_write_jpg(path, image->cols, image->rows, 3, flatmap, 80);
    printf("test %s %d\n", path, result);
}
//

int *create_gaussian_kernel(int size, int sigma, int *kernel_total)
{
    float sum = 0;
    // Lets get odd size to make the center item a one.
    // For ex, If size is 4 then, there is no single center item. If size is 5, (2,2) is the center.
    if(size % 2 == 0) {
        size++;
    }
    double *kernel = (double *)malloc(sizeof(double) * size * size);    
    int *intkernel = (int *)malloc(sizeof(int) * size * size);
    int center = size/2;
    double kernel_total_private = 0;
    int intkernel_total = 0;

    for(int i=0; i < size; i++)
    {
        for(int j=0; j < size; j++)
        {
            int x = i - center;
            int y = i - center;
            int exp_power = -1 * (x*x + y*y)/(2 * sigma * sigma);
            double gaussian_item = exp(exp_power)/ (2* 3.1416 * sigma * sigma);
            kernel[i*size+j] = gaussian_item;
            kernel_total_private += gaussian_item;
            printf("%f ", gaussian_item);            
        }
        printf("\n");
    }

    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            double item = kernel[i*size + j];
            item /= kernel_total_private;
            item *= 100;            
            intkernel[i*size + j] = item;
            intkernel_total += intkernel[i*size + j];
            printf("%d ", intkernel[i*size + j]);     
        }
        printf("\n");
    }

    *kernel_total = intkernel_total;
    printf("%d kernel total\n", intkernel_total);
    return intkernel;
}

void convolution_gaussian_blur(Matrix *image, int kernelsize)
{
    int kernal_total = 0;
    int *kernel = create_gaussian_kernel(kernelsize, 1, &kernal_total);
    convolution(image, kernel, kernelsize, kernal_total);
}

void convolution_sharpen(Matrix *image)
{
    double kernel[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
    convolution(image, kernel, 9, 10);
}

void convolution(Matrix *image, int *kernel, int size, int kernel_value)
{
    int kernel_length = size * size;

    for (int i = 0; i < image->rows; i++)
    {
        for (int j = 0; j < image->cols; j++)
        {
            int targetVal = mget(image, i, j), gausian_g = 0, gausian_b = 0, gausian_r = 0;
            int imgMatrix[9];

            for (int k = 0; k < kernel_length; k++)
            {
                imgMatrix[k] = mget(image, i - size/2 + k / size, j - size/2 + k % size);
                gausian_b += (imgMatrix[k] & 0xFF) * kernel[k];
                gausian_g += ((imgMatrix[k] >> 8) & 0xFF) * kernel[k];
                gausian_r += ((imgMatrix[k] >> 16) & 0xFF) * kernel[k];
            }
            gausian_b /= kernel_value;
            gausian_g /= kernel_value;
            gausian_r /= kernel_value;

            // printf("%d %d %d\n", gausian_r, gausian_g, gausian_b);

            targetVal = gausian_b + (gausian_g << 8) + (gausian_r << 16);
            mset(image, i, j, targetVal);
        }
    }
}

void convolution_infrared(Matrix *image)
{
    int kernel[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    for (int i = 0; i < image->rows; i++)
    {
        for (int j = 0; j < image->cols; j++)
        {
            int targetVal = mget(image, i, j), gausian_g = 0, gausian_b = 0, gausian_r = 0;
            int imgMatrix[9];

            for (int k = 0; k < 9; k++)
            {
                imgMatrix[k] = mget(image, i - 1 + k / 3, j - 1 + k % 3);
                gausian_b += (imgMatrix[k] & 0xFF) * kernel[k];
                gausian_g += ((imgMatrix[k] >> 8) & 0xFF) * kernel[k];
                gausian_r += ((imgMatrix[k] >> 16) & 0xFF) * kernel[k];
            }
            gausian_b /= 12;
            gausian_b = gausian_b;

            targetVal = gausian_b << 16 + gausian_b << 8 + gausian_b;
            mset(image, i, j, targetVal);
        }
    }
}

void monochrome(Matrix *image)
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
    Matrix *mImage = LoadImage("./images/img1.jpg");
    // save_image("./output/ar.jpg", mImage);
    convolution_gaussian_blur(mImage, 7);
    //monochrome(mImage);
    //convolution_sharpen(mImage);
    save_image("./output/img23.jpg", mImage);
}