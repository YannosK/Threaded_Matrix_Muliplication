#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define size 1000

void matrix_init_rand(int *first_addr, unsigned int dim);
void matrix_init_zeros(int *first_addr, unsigned int dim);
void matrix_printer(int *first_addr, unsigned int dim);

int main(void)
{
    int A[size][size];
    int B[size][size];
    int C[size][size];

    clock_t snap;
    long double elapsed;

    matrix_init_rand(&A[0][0], size);
    matrix_init_rand(&B[0][0], size);
    matrix_init_zeros(&C[0][0], size);

    // Matrix multiplication A * B = C
    snap = clock();
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    snap = clock() - snap;
    elapsed = ((long double)snap) * 1000000 / CLOCKS_PER_SEC;
    printf("\nSequential matrix multiplication of 4x4 matrixes took: %.2Lf ms\n", elapsed);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
// Function definitions
////////////////////////////////////////////////////////////////////////////////////////

void matrix_init_rand(int *first_addr, unsigned int dim)
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            *(first_addr + i * dim + j) = (rand() % 20) - 10;
        }
    }
}

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