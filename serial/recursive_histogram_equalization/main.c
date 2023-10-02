#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "helpers.h"
#include "matrix.h"

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
       if (iterations == 0)
       {
              return image;
       }
       else
       {
              Histogram *hist = get_histogram(image);
              CDF *cdf = cdf_compute_normalized(hist);
              apply_normalized_cdf(image, cdf);
              free(hist);
              free(cdf);
              recursive_histogram_equalization(image, iterations - 1);
       }
}

int main()
{
       clock_t start, end;

       /* Store start time here */
       start = clock();

       Matrix *image = load_image("./images/img1.jpg");
       recursive_histogram_equalization(image, 20);
       save_image("./output/img1.jpg", image);
       mfree(image);
       
       image = load_image("./images/img2.jpg");
       recursive_histogram_equalization(image, 20);
       save_image("./output/img2.jpg", image);
       mfree(image);
       
       image = load_image("./images/img3.jpg");
       recursive_histogram_equalization(image, 20);
       save_image("./output/img3.jpg", image);
       mfree(image);
       
       image = load_image("./images/img4.jpg");
       recursive_histogram_equalization(image, 20);
       save_image("./output/img4.jpg", image);
       mfree(image);

       end = clock();
       /* Get the time taken by program to execute in seconds */
       double duration = ((double)end - start) / CLOCKS_PER_SEC;

       printf("\n--------------------------------------------\nSerial Histogram Equalization Time  : %f", duration);
       return 0;
}