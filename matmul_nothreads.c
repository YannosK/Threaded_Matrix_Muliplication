#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000 // dimension of the square matrices (they are SIZE x SIZE)

void matrix_init_rand(int *first_addr, unsigned int dim);
void matrix_init_zeros(int *first_addr, unsigned int dim);
void matrix_printer(int *first_addr, unsigned int dim);

// I store them in static memory, else I run out of stack space
int A[SIZE][SIZE];
int B[SIZE][SIZE];
int C[SIZE][SIZE];

int main(void)
{
    // variables used for timing
    clockid_t clk_id = CLOCK_MONOTONIC;
    struct timespec tspec;
    long double dur;

    // matrix initialization
    matrix_init_rand(&A[0][0], SIZE);
    matrix_init_rand(&B[0][0], SIZE);
    matrix_init_zeros(&C[0][0], SIZE);

    // Matrix multiplication A * B = C
    clock_gettime(clk_id, &tspec);
    dur = (long double)(tspec.tv_sec) + ((long double)(tspec.tv_nsec) / 1000000000);
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            for (int k = 0; k < SIZE; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    clock_gettime(clk_id, &tspec);
    dur = ((long double)(tspec.tv_sec) + ((long double)(tspec.tv_nsec) / 1000000000)) - dur;
    printf("\nSequential matrix multiplication took: %.6Lf seconds\n", dur);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
// Function definitions
////////////////////////////////////////////////////////////////////////////////////////

/*
    initialises a square matrix to random default values
    Values are in the range -(RAND_MAX / 2) to (RAND_MAX / 2)
*/
void matrix_init_rand(int *first_addr, unsigned int dim)
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            *(first_addr + i * dim + j) = (rand() % RAND_MAX) - (RAND_MAX / 2);
        }
    }
}

/*
    initialises a square matrix to zero element values
*/
void matrix_init_zeros(int *first_addr, unsigned int dim)
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            *(first_addr + i * dim + j) = 0;
        }
    }
}

/*
    Prints a square matrix in the terminal
    WARNING:    since it prints out in the terminal matrixes with very large element values,
                or too many values, might not be printed correctly
*/
void matrix_printer(int *first_addr, unsigned int dim)
{
    for (int i = 0; i < dim; i++)
    {
        printf("\n|");
        for (int j = 0; j < dim; j++)
        {
            printf("%d\t", *(first_addr + i * dim + j));
        }
        printf("\b\b\b|");
    }
    printf("\n");
}