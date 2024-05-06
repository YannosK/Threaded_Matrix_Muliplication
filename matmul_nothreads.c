#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000

void matrix_init_rand(int *first_addr, unsigned int dim);
void matrix_init_zeros(int *first_addr, unsigned int dim);
void matrix_printer(int *first_addr, unsigned int dim);

// I store them in static memory, else I run out of stack space
int A[SIZE][SIZE];
int B[SIZE][SIZE];
int C[SIZE][SIZE];

int main(void)
{
    clock_t snap;
    long double elapsed;

    matrix_init_rand(&A[0][0], SIZE);
    matrix_init_rand(&B[0][0], SIZE);
    matrix_init_zeros(&C[0][0], SIZE);

    // Matrix multiplication A * B = C
    snap = clock();
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
    snap = clock() - snap;
    elapsed = ((long double)snap) * 1000 / CLOCKS_PER_SEC;
    printf("\nSequential matrix multiplication took: %.2Lf ms\n", elapsed);

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
            *(first_addr + i * dim + j) = (rand() % RAND_MAX) - (RAND_MAX / 2);
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