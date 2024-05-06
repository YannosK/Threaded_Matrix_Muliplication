#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define size 4

void *matrix_partial_multiplier(void *args);
void matrix_init_rand(int *first_addr, unsigned int dim);
void matrix_init_zeros(int *first_addr, unsigned int dim);
void matrix_printer(int *first_addr, unsigned int dim);

struct p
{
    int *A;
    int *B;
    int *C;
    unsigned int A_rows;
    unsigned int B_columns;
    unsigned int A_columns; // same as B rows
};
typedef struct p *params;

// I store them in static memory, else I run out of stack space
int A[size][size];
int B[size][size];
int C[size][size];

int main(void)
{
    clock_t snap;
    long double elapsed;
    // pthread_t t1, t2, t3, t4, t5, t6, t7, t8;

    matrix_init_rand(&A[0][0], size);
    matrix_init_rand(&B[0][0], size);
    matrix_init_zeros(&C[0][0], size);

    matrix_printer(&A[0][0], size);
    matrix_printer(&B[0][0], size);
    matrix_printer(&C[0][0], size);

    struct p p1;
    p1.A = &A[0][0];
    p1.B = &B[0][0];
    p1.C = &C[0][0];
    p1.A_rows = 2;
    p1.A_columns = 4;
    p1.B_columns = 4;
    params arg1 = &p1;

    struct p p2;
    p2.A = &A[2][0];
    p2.B = &B[0][0];
    p2.C = &C[2][0];
    p2.A_rows = 2;
    p2.A_columns = 4;
    p2.B_columns = 4;
    params arg2 = &p2;

    // Matrix multiplication A * B = C
    snap = clock();
    matrix_partial_multiplier((void *)arg1);
    snap = clock() - snap;
    elapsed = ((long double)snap) * 1000 / CLOCKS_PER_SEC;
    printf("\nSequential matrix multiplication of first half took: %.2Lf ms\n", elapsed);

    snap = clock();
    matrix_partial_multiplier((void *)arg2);
    snap = clock() - snap;
    elapsed = ((long double)snap) * 1000 / CLOCKS_PER_SEC;
    printf("\nSequential matrix multiplication of second half took: %.2Lf ms\n", elapsed);

    matrix_printer(&C[0][0], size);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
// Function definitions
////////////////////////////////////////////////////////////////////////////////////////

void *matrix_partial_multiplier(void *args)
{
    int *a = ((params)args)->A;
    int *b = ((params)args)->B;
    int *c = ((params)args)->C;
    unsigned int c_rows = ((params)args)->A_rows;
    unsigned int c_columns = ((params)args)->B_columns;
    unsigned int c_sum = ((params)args)->A_columns;

    for (int i = 0; i < c_rows; i++)
    {
        for (int j = 0; j < c_columns; j++)
        {
            for (int k = 0; k < c_sum; k++)
            {
                // C[i][j] += A[i][k] * B[k][j];
                // *(first_addr + i * dim + j)
                // (*(c + i * c_columns + j)) = (*(c + i * c_columns + j)) + (*(a + i * c_sum + k)) + (*(b + k * c_columns + j));
                (*(c + i * c_columns + j)) += (*(a + i * c_sum + k)) * (*(b + k * c_columns + j));
            }
        }
    }
    return NULL;
}

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