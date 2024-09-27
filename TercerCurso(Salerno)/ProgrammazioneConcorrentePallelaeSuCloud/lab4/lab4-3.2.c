#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(argc,argv) int argc; char *argv[];
{
    int numtasks, rank, dest, source, rc, count, tag=1;  
    MPI_Status Stat;
    
    int message;
    
    //Gather
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); //Ver el numero de procesos que hay
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("El proceso %d recibe los siguientes numeros: \n", rank);
        for (int i = 1; i < numtasks; i++) {
            MPI_Recv(&message, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);
            printf("%d recibido del proceso: %d, ",message, i);
        }
    } else {
        source = 0;
        message = rank;
        rc = MPI_Send(&message, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
