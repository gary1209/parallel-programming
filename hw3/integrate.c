#include <stdio.h>
#include <math.h>
#include <mpi.h>
#define PI 3.1415926535

int main(int argc, char **argv) 
{
  long long i, num_intervals, upper_bound, lower_bound, avg;
  double rect_width, area, sum, x_middle, local_sum; 
  int size, rank;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  sscanf(argv[1],"%llu",&num_intervals);

  rect_width = PI / num_intervals;
  avg = (long long) num_intervals / size;
  lower_bound = avg * rank + 1;
  upper_bound = lower_bound + avg -1;
  sum = 0;

  if(rank == 0){
    for(i = lower_bound; i < upper_bound; i++) {

      /* find the middle of the interval on the X-axis. */ 

      x_middle = (i - 0.5) * rect_width;
      area = sin(x_middle) * rect_width; 
      sum = sum + area;
    } 
    for(int j = 1; j < size; j++){
      MPI_Recv(&local_sum, 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, &status);
      sum += local_sum;
    }
    printf("The total area is: %f\n", (float)sum);
  }

  if(rank != 0){
    for(i = lower_bound; i < upper_bound; i++) {
      x_middle = (i - 0.5) * rect_width;
      area = sin(x_middle) * rect_width;
      sum = sum + area;
    }
    MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}   
