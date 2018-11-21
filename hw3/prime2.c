#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>


int isprime(long long n) {
  int i,squareroot;
  if (n>10) {
    squareroot = (int) sqrt(n);
    for (i=3; i<=squareroot; i=i+2)
      if ((n%i)==0)
        return 0;
    return 1;
  }
  else
    return 0;
}

int main (int argc, char *argv[])
{
  int size;
  int rank;
  int pc = 0;
  int pcsum;
  int stride;
  long long limit;
  long long n;
  long long maxprime;
  long long foundone = 0;
  long long mystart;

  sscanf(argv[1],"%llu",&limit);
  
  if (((size%2) !=0) || ((limit%size) !=0)) {
   printf("Sorry - this exercise requires an even number of tasks.\n");
   printf("evenly divisible into %d.  Try 4 or 8.\n",limit);
   MPI_Finalize();
   exit(0);
   }

  
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  mystart = (rank*2)+1;       /* Find my starting point - must be odd number */
  stride = size*2;          /* Determine stride, skipping even numbers */

  if (rank == 0) {
     printf("Starting. Numbers to be scanned= %lld\n",limit);
     for (n=mystart; n<=limit; n=n+stride) {
        if (isprime(n)) {
           pc++;
           foundone = n;
           }
        }
     MPI_Reduce(&pc,&pcsum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
     MPI_Reduce(&foundone,&maxprime,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
     //pcsum + 4 for 2,3,5,7
     printf("Done. Largest prime is %lld Total primes %d\n",maxprime,pcsum + 4);
     }


  if (rank > 0) {
     for (n=mystart; n<=limit; n=n+stride) {
        if (isprime(n)) {
           pc++;
           foundone = n;
           }
        }
     MPI_Reduce(&pc,&pcsum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
     MPI_Reduce(&foundone,&maxprime,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
     }

  MPI_Finalize();
  }
