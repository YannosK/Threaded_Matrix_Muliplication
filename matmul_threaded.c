#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>

#define SIZE 2
#define THREAD_NUM 2

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
    assert(SIZE % THREAD_NUM == 0 && SIZE >= THREAD_NUM);

    // variables used for timing
    clock_t snap;
    long double elapsed;

    // variables used for partial multiplication functions
    struct p p_structs[THREAD_NUM];
    params arg[THREAD_NUM];

    // variables used in the threads
    pthread_t t[THREAD_NUM];
    int *ret[THREAD_NUM];

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

    snap = clock();
    // Creating the threads
    for (int n = 0; n < THREAD_NUM; n++)
        pthread_create(&(t[n]), NULL, matrix_partial_multiplier, (void *)(arg[n]));
    // Waiting for them to finish
    for (int n = 0; n < THREAD_NUM; n++)
        pthread_join(t[n], (void **)(&(ret[n])));
    snap = clock() - snap;
    elapsed = ((long double)snap) * 1000 / CLOCKS_PER_SEC;
    printf("\nThreaded matrix multiplication took: %.2Lf ms\n", elapsed);

    matrix_printer(&A[0][0], SIZE);
    matrix_printer(&B[0][0], SIZE);
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