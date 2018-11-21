#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

typedef long long ll;

int isprime(ll n) {
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

int main(int argc, char *argv[])
{
  int pc;     /* prime counter */
  ll  foundone; /* most recent prime found */
  int gap;
  int size;
  int rank;
  int tag = 0;
  ll n, limit, avg, up_bound, recv_buf;

  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  sscanf(argv[1],"%llu",&limit);
  
  pc=0;

  avg = (ll)(limit/size);

  if(rank == size -1){ //last process
    up_bound = limit;
  }else{
    up_bound = avg * (rank + 1);
  }

  if((avg*rank+1)%2 == 0){
    n = avg * rank + 2;
  }else{
    n = avg * rank + 1;
  }

  for(n; n <= up_bound; n+=2){
    if ((n % 6 ==1) || (n % 6 == 5)){
      if (isprime(n)) {
        pc++;
        foundone = n;
      } 
    }		
  }
  
  if(rank == size-1){
    printf("Starting. Numbers to be scanned= %lld\n",limit);
    for(int i = 0; i < rank; i++){
      MPI_Recv(&recv_buf, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
      pc += recv_buf;
    }
    printf("Done. Largest prime is %lld Total primes %d\n",foundone,pc+4);
  }else{
    MPI_Send(&pc, 1, MPI_INT, size-1, tag, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
} 
