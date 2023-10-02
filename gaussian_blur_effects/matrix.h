// Define the matrix structure
typedef struct {
    int rows;
    int cols;
    int **data;  // Pointer to a 2D array of integers
} Matrix;

Matrix *create_matrix(int rows, int cols);
int mget(Matrix *matrix, int row, int col);
void mset(Matrix *matrix, int row, int col, int value);
void mfree(Matrix *matrix);
int *mflat(Matrix *matrix);
char *mflatRGB(Matrix *matrix, int nComps);