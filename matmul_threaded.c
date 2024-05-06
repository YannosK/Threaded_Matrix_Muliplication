#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>

#define SIZE 4
#define THREAD_NUM 4

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
int A[SIZE][SIZE];
int B[SIZE][SIZE];
int C[SIZE][SIZE];

int main(void)
{
    // some testing for my defines
    assert(SIZE % THREAD_NUM == 0);

    // variable decleration
    clock_t snap;
    long double elapsed;

    struct p p_structs[THREAD_NUM];
    params arg[THREAD_NUM];

    // pthread_t t[2];

    // matrix initialization
    matrix_init_rand(&A[0][0], SIZE);
    matrix_init_rand(&B[0][0], SIZE);
    matrix_init_zeros(&C[0][0], SIZE);

    // arguments of threaded functions
    const int m = SIZE / THREAD_NUM;
    for (int i = 0; i < THREAD_NUM; i++)
    {
        p_structs[i].A = &A[m * i][0];
        p_structs[i].B = &B[0][0];
        p_structs[i].C = &C[m * i][0];
        p_structs[i].A_rows = m;
        p_structs[i].A_columns = SIZE;
        p_structs[i].B_columns = SIZE;
        arg[i] = &p_structs[i];
    }

    // Matrix multiplication A * B = C

    matrix_printer(&A[0][0], SIZE);
    matrix_printer(&B[0][0], SIZE);
    matrix_printer(&C[0][0], SIZE);

    for (int l = 0; l < THREAD_NUM; l++)
    {
        snap = clock();
        matrix_partial_multiplier((void *)(arg[l]));
        snap = clock() - snap;
        elapsed = ((long double)snap) * 1000 / CLOCKS_PER_SEC;
        printf("\nSequential matrix multiplication of %d half took: %.2Lf ms\n", l, elapsed);
    }

    matrix_printer(&C[0][0], SIZE);

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