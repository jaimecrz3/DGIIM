#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[])
{
int numtasks, rank, dest, tag = 1, source, rc, count;
int size = atoi(argv[1]);
MPI_Status Stat;
MPI_Request request = MPI_REQUEST_NULL;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

int array[size];
for(int i=0; i<size; i++){
  array[i] = i;
}


int max=array[0], min=array[0];

int levels;
if(size%numtasks==0){
  levels=size/numtasks;
}else if(rank == numtasks-1){
  levels=size/numtasks + (size%numtasks);
} else{
  levels=size/numtasks;
}
//int levels=size/numtasks;
MPI_Barrier(MPI_COMM_WORLD);
for(int i=rank*(size/numtasks); i<rank*(size/numtasks)+levels; i++){
    if(max < array[i]) {
        max=array[i];
    }
    if(min > array[i]) {
        min=array[i];  
     } 
}

MPI_Barrier(MPI_COMM_WORLD);
int minimo, maximo;
//Comunicación blocante
if (rank == 0) {
        maximo = max;
        minimo = min;
        for (int i = 1; i < numtasks; i++) {
            MPI_Recv(&max, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);
            if(maximo < max) {
                maximo=max; 
            } 
            MPI_Recv(&min, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);
            if(minimo > min){
                minimo=min;
            }
        }
        
} else {
        MPI_Send(&max, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        MPI_Send(&min, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
}

if(rank==0){
    printf("El minimo es: %d y el maximo es: %d\n", minimo, maximo);
}

MPI_Finalize();
}
