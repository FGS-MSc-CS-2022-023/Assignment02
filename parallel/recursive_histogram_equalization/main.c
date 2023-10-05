#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "helpers.h"
#include "matrix.h"
#include "omp.h"

typedef struct
{
       int RChannel[256];
       int GChannel[256];
       int BChannel[256];
} Histogram;

typedef struct
{
       double RChannel[256];
       double GChannel[256];
       double BChannel[256];
} CDF;

Histogram *get_histogram(Matrix *image)
{
       Histogram *hist = (Histogram *)malloc(sizeof(Histogram));
       memset(hist, 0, sizeof(Histogram));
       for (int i = 0; i < image->rows; i++)
       {
              for (int j = 0; j < image->cols; j++)
              {
                     int pixel = mget(image, i, j);
                     int r, g, b;
                     b = pixel & 0xFF;
                     g = (pixel >> 8) & 0xFF;
                     r = (pixel >> 16) & 0xFF;
                     hist->RChannel[r]++;
                     hist->GChannel[g]++;
                     hist->BChannel[b]++;
              }
       }
       return hist;
}

void free_histogram(Histogram *hist)
{
       free(hist);
}

CDF *cdf_compute_normalized(Histogram *hist)
{
       //printf("t -> %d, ", omp_get_thread_num());
       int *channels[3] = {hist->RChannel, hist->GChannel, hist->BChannel};
       CDF *cdf = (CDF *)malloc(sizeof(CDF));
       double *cdfChannels[3] = {cdf->RChannel, cdf->GChannel, cdf->BChannel};
       for (int i = 0; i < 3; i++)
       {
              int startingValue = channels[i][0];
              cdfChannels[i][0] = startingValue;
              for (int j = 1; j < 256; j++)
              {
                     startingValue += channels[i][j];
                     cdfChannels[i][j] = startingValue;
              }
       }
       for (int i = 0; i < 3; i++)
       {
              double final_val = cdfChannels[i][255];
              for (int j = 0; j < 256; j++)
              {
                     cdfChannels[i][j] = cdfChannels[i][j] * 255 / final_val;
              }
       }

       return cdf;
}

void apply_normalized_cdf(Matrix *matrix, CDF *cdf)
{
       //#pragma omp parallel for
       for (int i = 0; i < matrix->rows; i++)
       {
              for (int j = 0; j < matrix->cols; j++)
              {
                     int item = mget(matrix, i, j);

                     int r, g, b;
                     r = (item & 0xFF0000) >> 16;
                     g = (item & 0xFF00) >> 8;
                     b = (item & 0xFF);
                     int normalized_r = cdf->RChannel[r];
                     int normalized_g = cdf->GChannel[g];
                     int normalized_b = cdf->BChannel[b];
                     int normalized_rgb = normalized_b + (normalized_g << 8) + (normalized_r << 16);

                     mset(matrix, i, j, normalized_rgb);
              }
       }
}

void free_cdf(CDF *cdf)
{
       free(cdf);
}

void recursive_histogram_equalization(Matrix *image, int iterations)
{
       if(iterations == 0)
       {
              return;
       }
       Histogram *hist;
       CDF *cdf;   
       #pragma omp task shared(image, hist, cdf) untied if(iterations > 0)
       {  
              //printf("t2 -> %d, ", omp_get_thread_num());
              recursive_histogram_equalization(image, iterations - 1);
              hist = get_histogram(image);
              cdf = cdf_compute_normalized(hist);
       }
       #pragma omp taskwait
       apply_normalized_cdf(image, cdf);
       free_cdf(cdf);
       free_histogram(hist);
}

int main()
{
       double tstart, tcalc, tstop;
       tstart = omp_get_wtime();
       int nthreads = 0;

       #pragma omp parallel
       {
              nthreads = omp_get_num_threads();
              #pragma omp single
              {
                     
                     int files;
                     printf("main -> %d, ", omp_get_thread_num());
                     char **images = get_files("./images", &files);
                     for (int i = 0; i < files; i++)
                     {                   
                            char filename[260];
                            sprintf(filename, "%s/%s", "./images", images[i]);
                            Matrix *image = load_image(filename);
                            recursive_histogram_equalization(image, 400);
                            sprintf(filename, "%s/%s", "./output", images[i]);
                            save_image(filename, image);
                            mfree(image);
                     }

              }
       }

       tstop = omp_get_wtime();
       tcalc = tstop - tstart;

       printf("Parallel Histogram Equalization Time  : %f, threads: %d\n", tcalc, omp_get_max_threads());
       return 0;
}
