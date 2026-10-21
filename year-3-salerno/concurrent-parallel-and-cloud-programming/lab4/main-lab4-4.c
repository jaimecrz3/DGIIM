#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab4-4.h"

int main(argc,argv) int argc; char *argv[];
{

  int size = atoi(argv[1]);
  MPI_Request request = MPI_REQUEST_NULL;
  MPI_Status status;
  MPI_Init(&argc,&argv);
  
  MyCollective my_collective;
  my_collective_init(&my_collective);
  
  /*int N = 100;
  int message[N];
  
  if(my_collective.rank == 0){
    for(int i=0; i<N; i++){
          message[i] = i;
    }
  }*/
  
  
  /*if(my_collective.rank == 0){
    printf("Broadcast de los datos: \n");
  }
  my_broadcast(&my_collective, message, N, 0);*/
  
  /*if(my_collective.rank == 0){
    printf("Gather de los datos: \n");
  }
  my_gather(&my_collective, message, 0);*/
  
  /*if(my_collective.rank == 0){
    printf("Scatter de los datos: \n");
  }
  my_scatter(&my_collective, message, 0);*/
  
  /*int array[size];
  for(int i=0; i<size; i++){
    array[i] = i;
  }
  //printf("%d: ", size);
  MPI_Barrier(my_collective.comm);
  my_reduction(&my_collective, array, size);*/
  
  
  /*if(my_collective.rank == 0){
    printf("Broadcast de los datos: \n");
  }
  my_broadcast_non_blocking(&my_collective, message, N, 0, &request, &status);
  MPI_Wait(&request, &status);
*/

  /*if(my_collective.rank == 0){
    printf("Scatter de los datos: \n");
  }
  my_gather_non_blocking(&my_collective, message, 0, &request, &status);
  MPI_Wait(&request, &status);*/
  
  /*if(my_collective.rank == 0){
    printf("Scatter de los datos: \n");
  }
  my_scatter_non_blocking(&my_collective, message, 0, &request, &status);
  MPI_Wait(&request, &status);*/
  
  int array[size];
  for(int i=0; i<size; i++){
    array[i] = i;
  }
  //printf("%d: ", size);
  MPI_Barrier(my_collective.comm);
  my_reduction_non_blocking(&my_collective, array, size, &request, &status);
  MPI_Wait(&request, &status);
  
  MPI_Finalize();
  
}
