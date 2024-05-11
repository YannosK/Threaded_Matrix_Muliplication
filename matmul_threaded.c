#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>

#define SIZE 1000    // dimension of the square matrices (they are SIZE x SIZE)
#define THREAD_NUM 8 // number of threads to run the matrix multiplication. Must devide SIZE exactly!

void *matrix_partial_multiplier(void *args);
void matrix_init_rand(int *first_addr, unsigned int dim);
void matrix_init_zeros(int *first_addr, unsigned int dim);
void matrix_printer(int *first_addr, unsigned int dim);

/*
    struct containning the arguments that are required by the function matrix_partial_multiplier

*/
struct p
{
    int *A;                 // address of first element of matrix A (element A[0][0])
    int *B;                 // address of first element of matrix B (element B[0][0])
    int *C;                 // address of first element of matrix C (element C[0][0])
    unsigned int A_rows;    // number of rows of matrix A. Same as the number of rows of result matrix C
    unsigned int B_columns; // number of columns of matrix B. Same as the number of columns of result matrix C
    unsigned int A_columns; // number of columns of matrix A. Same as B rows
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
    clockid_t clk_id = CLOCK_MONOTONIC;
    struct timespec tspec;
    long double dur;

    // variables used for partial multiplication functions
    // creating an array of argument structs, each one for every thread
    struct p p_structs[THREAD_NUM];
    params arg[THREAD_NUM];

    // variables used in the threads
    // again arrays of argument structs, each used by one thread
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

    clock_gettime(clk_id, &tspec);
    dur = (long double)(tspec.tv_sec) + ((long double)(tspec.tv_nsec) / 1000000000);
    // Creating the threads
    for (int n = 0; n < THREAD_NUM; n++)
        pthread_create(&(t[n]), NULL, matrix_partial_multiplier, (void *)(arg[n]));
    // Waiting for them to finish
    for (int n = 0; n < THREAD_NUM; n++)
        pthread_join(t[n], (void **)(&(ret[n])));
    clock_gettime(clk_id, &tspec);
    dur = ((long double)(tspec.tv_sec) + ((long double)(tspec.tv_nsec) / 1000000000)) - dur;
    printf("\nThreaded matrix multiplication took: %.6Lf seconds\n", dur);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
// Function definitions
////////////////////////////////////////////////////////////////////////////////////////

/*
    Performs matrix multiplication of A (MxN) and B(NxK) to produce C(MxK)
    It takes as argument a null pointer to a struct
    The struct must be of type: struct p, with pointer to it being a defined type: params
    It has the structure required for functions to be passed as arguments in the pthreads library's functions
*/
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
                (*(c + i * c_columns + j)) += (*(a + i * c_sum + k)) * (*(b + k * c_columns + j));
            }
        }
    }
    return NULL;
}

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