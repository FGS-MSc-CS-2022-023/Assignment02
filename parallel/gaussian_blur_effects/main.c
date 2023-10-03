#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "helpers.h"
#include "convolutions.h"
#include <omp.h>

void work()
{
    int len;
    char **files = get_files("./images", &len);

    for (int i = 0; i < len; i++)
    {
        char input[260];
        char output[260];
        sprintf(input, "%s/%s", "./images", files[i]);
        printf("%s", input);
        Matrix *mImage = load_image(input);
        convolution_gaussian_blur(mImage, 5);
        convolution_gaussian_blur(mImage, 7);
        sprintf(output, "%s/gaussian_blur_%s", "./output", files[i]);
        save_image(output, mImage);
        mfree(mImage);

        mImage = load_image(input);
        grayscale(mImage);
        sprintf(output, "./output/grayscale_%s", files[i]);
        save_image(output, mImage);
        mfree(mImage);

        mImage = load_image(input);
        convolution_infrared(mImage);
        sprintf(output, "./output/sharpinfra_%s", files[i]);
        save_image(output, mImage);
        mfree(mImage);

    }
    
}

int main()
{
    double tstart, tcalc, tstop;
    tstart = omp_get_wtime();
    int nthreads = 0;

    work();

    tstop = omp_get_wtime();
    tcalc = tstop - tstart;

    printf("\n--------------------------------------------\nParallel Gausian Blur Effects Time  : %f, nthreads: %d\n", tcalc, omp_get_max_threads());
    return 0;
}