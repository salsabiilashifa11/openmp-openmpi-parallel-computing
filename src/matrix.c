#include "matrix.h"
#include <time.h>


/* 
 * Procedure init_matrix
 * 
 * Initializing newly allocated matrix
 * Setting all data to 0 and effective dimensions according
 * to nrow and ncol 
 * */
void init_matrix(Matrix *m, int nrow, int ncol) {
	m->row_eff = nrow;
	m->col_eff = ncol;

	for (int i = 0; i < m->row_eff; i++) {
		for (int j = 0; j < m->col_eff; j++) {
			m->mat[i][j] = 0;
		}
	}
}


/* 
 * Function input_matrix
 *
 * Returns a matrix with values from stdin input
 * */
Matrix input_matrix(int nrow, int ncol) {
	Matrix input;
	init_matrix(&input, nrow, ncol);

	for (int i = 0; i < nrow; i++) {
		for (int j = 0; j < ncol; j++) {
			scanf("%d", &input.mat[i][j]);
		}
	}

	return input;
}


/* 
 * Procedure print_matrix
 * 
 * Print matrix data
 * */
void print_matrix(Matrix *m) {
	for (int i = 0; i < m->row_eff; i++) {
		for (int j = 0; j < m->col_eff; j++) {
			printf("%d ", m->mat[i][j]);
		}
		printf("\n");
	}
}


/* 
 * Function get_matrix_datarange
 *
 * Returns the range between maximum and minimum
 * element of a matrix
 * */
int get_matrix_datarange(Matrix *m) {
	int max = DATAMIN;
	int min = DATAMAX;
	for (int i = 0; i < m->row_eff; i++) {
		for (int j = 0; j < m->col_eff; j++) {
			int el = m->mat[i][j];
			if (el > max) max = el;
			if (el < min) min = el;
		}
	}

	return max - min;
}


/*
 * Function supression_op
 *
 * Returns the sum of intermediate value of special multiplication
 * operation where kernel[0][0] corresponds to target[row][col]
 * */
int supression_op(Matrix *kernel, Matrix *target, int row, int col, int n_thread) {
	int temp, intermediate_sum;

	temp = 0;
	intermediate_sum = 0;
	
	for (int i = 0; i < kernel->row_eff; i++) {
		for (int j = 0; j < kernel->col_eff; j++) {
			temp += kernel->mat[i][j] * target->mat[row+i][col+j];
		}
	}

	return temp;
}

/* 
 * Function convolution
 *
 * Return the output matrix of convolution operation
 * between kernel and target
 * */
Matrix convolution(Matrix *kernel, Matrix *target, int n_thread) {
	// clock_t begin = clock();
	#pragma omp num_threads(n_thread) for nowait parallel
	{
		Matrix out;
		int out_row_eff = target->row_eff - kernel->row_eff + 1;
		int out_col_eff = target->col_eff - kernel->col_eff + 1;
		
		init_matrix(&out, out_row_eff, out_col_eff);
		
		for (int i = 0; i < out.row_eff; i++) {
			for (int j = 0; j < out.col_eff; j++) {
				out.mat[i][j] = supression_op(kernel, target, i, j, n_thread);
			}
		
		}

		return out;
	}
	
	

	// clock_t end = clock();

	// double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	// printf("Execution Time: %f sec\n", time_spent);
	
	
}

/*
 * find_range
 *
*/
int find_range(Matrix *m, int n_thread) {
	int max = DATAMIN;
	int min = DATAMAX;
	
	for (int i = 0; i < m->row_eff; i++) {
			for (int j = 0; j < m->col_eff; j++) {
				int el = m->mat[i][j];
				if (el > max) max = el;
				if (el < min) min = el;
			}
	}

	return max - min;
}

/*
 * Procedure merge_array
 *
 * Merges two subarrays of n with n[left..mid] and n[mid+1..right]
 * to n itself, with n now ordered ascendingly
 * */
void merge_array(int *n, int left, int mid, int right) {
	#pragma omp task 
	{
		int n_left = mid - left + 1;
		int n_right = right - mid;
		int iter_left = 0, iter_right = 0, iter_merged = left;
		int arr_left[n_left], arr_right[n_right];

		for (int i = 0; i < n_left; i++) {
			arr_left[i] = n[i + left];
		}

		for (int i = 0; i < n_right; i++) {
			arr_right[i] = n[i + mid + 1];
		}

		while (iter_left < n_left && iter_right < n_right) {
			if (arr_left[iter_left] <= arr_right[iter_right]) {
				n[iter_merged] = arr_left[iter_left++];
			} else {
				n[iter_merged] = arr_right[iter_right++];
			}
			iter_merged++;
		}

		while (iter_left < n_left)  {
			n[iter_merged++] = arr_left[iter_left++];
		}
		while (iter_right < n_right) {
			n[iter_merged++] = arr_right[iter_right++];
		}
	} 
}


/* 
 * Procedure merge_sort
 *
 * Sorts array n with merge sort algorithm
 * */
void merge_sort(int *n, int left, int right) {
	// #pragma omp parallel
  // #pragma omp single nowait
	if (left < right) {
		int mid = left + (right - left) / 2;

		#pragma omp task
		merge_sort(n, left, mid);
		#pragma omp task
		merge_sort(n, mid + 1, right);

		merge_array(n, left, mid, right);
	}
}
 

/* 
 * Procedure print_array
 *
 * Prints all elements of array n of size to stdout
 * */
void print_array(int *n, int size) {
	for (int i = 0; i < size; i++ ) printf("%d ", n[i]);
	printf("\n");
}


/* 
 * Function get_median
 *
 * Returns median of array n of length
 * */
int get_median(int *n, int length) {
	int mid = length / 2;
	if (length & 1) return n[mid];

	return (n[mid - 1] + n[mid]) / 2;
}


/* 
 * Function get_floored_mean
 *
 * Returns floored mean from an array of integers
 * */
long get_floored_mean(int *n, int length) {
	long sum = 0;
	for (int i = 0; i < length; i++) {
		sum += n[i];
	}

	return sum / length;
}

/* 
 * Function distribute_matrix
 *
 * Scatter matrix into several processes in parallel
 * */
void distribute_matrix(Matrix *matrix_process, Matrix* input_arr, int world_size, int n_targets, int target_col, int target_row){
	int idx = 0;
	int destination = 0;

	int i = 0;
	while (i < n_targets){
		destination = i % world_size;

		if (destination != 0){
              for (int j = 0; j < target_row; j++){
                  //Send matriks to destination Rank
                  MPI_Send(
                      input_arr[i].mat[j],
                      target_col,
                      MPI_INT,
                      destination,
                      0,
                      MPI_COMM_WORLD
                  );
              }
          } 
          else {
              matrix_process[idx] = input_arr[i];
              idx++;
          }
		i ++;
	}
}

/* 
 * Function receive_matrix
 *
 * Scatter matrix into several processes in parallel
 * */
void receive_matrix(Matrix *matrix_process, int n_threads, int target_col, int target_row ){
	
	for (int i = 0; i < n_threads; i ++){
		init_matrix(
			&matrix_process[i],
			target_row,
			target_col
		);

		for (int j = 0; j < target_row; j++){
			
			MPI_Recv(
					matrix_process[i].mat[j],
					target_col,
					MPI_INT,
					0,
					0,
					MPI_COMM_WORLD,
					MPI_STATUS_IGNORE
			);
		}
	}
}

void gather_matrix(int *combined_range_result, int *range_result, int world_size, int n_targets){
	int thread_number = 0;

    //Put in local data range to global data range
    for (int i = 0; i < n_targets; i++){
        int destination = i % world_size;

        if (destination != 0){
            //Send data range to destination Rank
            MPI_Recv(
                &combined_range_result[i],
                1,
                MPI_INT,
                destination,
                0,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );
            
        } else {
            combined_range_result[i] = range_result[thread_number];
            thread_number++;
        }
    }

}

