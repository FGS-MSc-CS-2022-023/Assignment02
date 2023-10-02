#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "helpers.h"
#include "convolutions.h"

void work()
{
    char *files[4] = {"img1.jpg", "img2.jpg", "img3.jpg", "img4.jpg"};
    char inputname[50], outputname[100];

    for (int i = 1; i <= 4; i++)
    {
        sprintf(inputname, "./images/img%d.jpg", i);
        Matrix *mImage = load_image(inputname);
        convolution_gaussian_blur(mImage, 5);
        sprintf(outputname, "./output/gaussian_blur_img%d.jpg", i);
        save_image(outputname, mImage);
        mfree(mImage);

        mImage = load_image(inputname);
        grayscale(mImage);
        sprintf(outputname, "./output/grayscale_img%d.jpg", i);
        save_image(outputname, mImage);
        mfree(mImage);

        mImage = load_image(inputname);
        convolution_infrared(mImage);
        sprintf(outputname, "./output/sharpinfrared_img%d.jpg", i);
        save_image(outputname, mImage);
        mfree(mImage);
    }
}

int main()
{
    clock_t start, end;

    /* Store start time here */
    start = clock();

    work();

    end = clock();
    /* Get the time taken by program to execute in seconds */
    double duration = ((double)end - start) / CLOCKS_PER_SEC;

    printf("\n--------------------------------------------\nSerial Gausian Blur Effects Time  : %f\n", duration);
    return 0;
}