#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <mpi.h>
#include <omp.h>
#include <string.h>
#include <stdlib.h>

#define NMAX 100
#define DATAMAX 1000
#define DATAMIN -1000

typedef struct Matrix {
	int mat[NMAX][NMAX];	// Matrix cells
	int row_eff;			// Matrix effective row
	int col_eff;			// Matrix effective column
} Matrix;

void init_matrix(Matrix *m, int nrow, int ncol);
Matrix input_matrix(int nrow, int ncol);
void print_matrix(Matrix *m);
int get_matrix_datarange(Matrix *m);
int supression_op(Matrix *kernel, Matrix *target, int row, int col, int n_thread);
Matrix convolution(Matrix *kernel, Matrix *target, int n_thread);
int find_range(Matrix *m, int n_thread);
void merge_array(int *n, int left, int mid, int right);
void merge_sort(int *n, int left, int right);
void print_array(int *n, int size);
int get_median(int *n, int length);
long get_floored_mean(int *n, int length);
int get_num_thread(int idx, int num_input, int num_process);
void distribute_matrix(Matrix *matrix_process, Matrix* input_arr, int world_size, int num_targets, int target_col, int target_row);
void receive_matrix(Matrix *matrix_process, int n_threads, int target_col, int target_row );
void gather_matrix(int *combined_range_result, int *range_result, int world_size, int n_targets);

#endif
