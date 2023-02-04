#include "matrix.h"
#include <time.h>

/*---------------------------------------------------*/
int world_size;
int world_rank;
int kernel_row, kernel_col, target_row, target_col, n_targets;
int n_threads;
Matrix kernel;
Matrix *input_arr;
Matrix *matrix_process;

int main(){
  
	/*
	* Start time
	*/
  clock_t begin = clock();

	
  //Initialize  MPI
  MPI_Init(NULL, NULL);

  //Initialize world_rank and number of process
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  /*
    Receiving kernel and target matrix input in the master process
  */
	if (world_rank == 0){
		//Kernel matrix input
		scanf("%d %d", &kernel_row, &kernel_col);
		kernel = input_matrix(kernel_row, kernel_col);

		//Target matrix input
		scanf("%d %d %d", &n_targets, &target_row, &target_col);
		input_arr = (Matrix*)malloc(n_targets * sizeof(Matrix));

		for (int i = 0; i < n_targets; i++) {
			input_arr[i] = input_matrix(target_row, target_col);
		}	
	}	

  /*
		Broadcast matrix kernel to all processes
	*/
  MPI_Bcast(&kernel.row_eff, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&kernel.col_eff, 1, MPI_INT, 0, MPI_COMM_WORLD);

	/*
		Broadcast target matrix to all processes
	*/
  MPI_Bcast(&target_col, 1, MPI_INT,  0, MPI_COMM_WORLD);
  MPI_Bcast(&target_row, 1, MPI_INT,  0, MPI_COMM_WORLD);

  /*
		Broadcast number of inputs
	*/
  MPI_Bcast(&n_targets, 1, MPI_INT, 0, MPI_COMM_WORLD);

  for (int i=0; i<kernel.row_eff; i++) {
    MPI_Bcast(kernel.mat[i], kernel.col_eff, MPI_INT, 0, MPI_COMM_WORLD);
  }

  /*
		Count the number of threads in each process
	*/
  int n_threads = 0;

	int i = 0;
	while (i < n_targets){
		if (i % world_size == world_rank){
			n_threads ++;
		}
		i ++;
	}

  /*
		Count the number of threads in each process
	*/  
  matrix_process = (Matrix*) malloc(n_threads * sizeof(Matrix));

  //Distribute matrix
  //Start from root process as sender


  if (world_rank == 0){
		distribute_matrix(
			matrix_process, 
			input_arr, 
			world_size, 
			n_targets, 
			target_col, 
			target_row
		);
  } else {
			//Other process as receiver
      receive_matrix(
        matrix_process, 
        n_threads, 
        target_col, 
        target_row
      );
  }


  
  //Initialized data range
  int range_result[n_threads];


  //openMP// Ini harusnya bagian openmp, tapi convolutionnya belom make yang openmp
  #pragma omp parallel num_threads(n_threads)
  {
    
    int thread_id = omp_get_thread_num();
    //Matrix kernel, Matrix target, int n_thread
    matrix_process[thread_id] = convolution(&kernel, &matrix_process[thread_id], 16);
    range_result[thread_id] = find_range(&matrix_process[thread_id], 16);
  }

  //Merge data range
  //Initialized data range and thread number
  int combined_range_result[n_targets];

  if (world_rank != 0){

    for (int i = 0; i < n_threads; i++){

      MPI_Send(
          &range_result[i],
          1,
          MPI_INT,
          0,
          0,
          MPI_COMM_WORLD
      );
    }
  }
  else {

    gather_matrix(
      combined_range_result,
      range_result, 
      world_size, 
      n_targets
    );

    merge_sort(combined_range_result, 0, n_targets-1);

		/*
			Finish Time
		*/
    clock_t end = clock();

	  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	// printf("Execution Time: %f sec\n", time_spent);


		printf("Execution Time: %f\n", time_spent);
    printf("Minimum: %d\n", combined_range_result[0]);
    printf("Maksimum: %d\n", combined_range_result[n_targets-1]);
    printf("Median: %d\n", get_median(combined_range_result, n_targets));
    printf("Mean: %ld\n", get_floored_mean(combined_range_result, n_targets));
  }

  // Finalize: Free any resources allocated
  MPI_Finalize();

  return 0;

}

// HOW TO RUN IN TERMINAL WSL
// mpicc -o parallel parallel-all.c --openmp
// mpirun -np 2 parallel