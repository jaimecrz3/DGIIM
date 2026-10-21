#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(argc,argv) int argc; char *argv[];
{
    int numtasks, rank, dest, source, rc, count, tag=1;  
    MPI_Status Stat;
    
    int message[100];
    int N = 100;
    
    //Broadcast
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); //Ver el numero de procesos que hay
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for(int i=0; i<100; i++){
          message[i] = i;
        }
    
        for (int i = 1; i < numtasks; i++) {
            MPI_Send(&message, N, MPI_INT, i, tag, MPI_COMM_WORLD);
        }
    } else {
        source = 0;
        rc = MPI_Recv(&message, N, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
        printf("Process %d received:\n", rank);
        for(int i=0; i<100; i++){
          printf("%d, ",message[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
}
