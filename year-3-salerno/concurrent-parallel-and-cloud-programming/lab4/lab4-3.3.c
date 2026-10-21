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
    
    //Scatter
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); //Ver el numero de procesos que hay
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int num_ints_per_process= 100/(numtasks-1);

    if (rank == 0) {
        for(int i=0; i<100; i++){
          message[i] = i;
        }
        int offset = 0;
        int cont;
        for (int i = 1; i < numtasks; i++) {
            for (int j=offset; j < offset + num_ints_per_process; j++){
              MPI_Send(&message[j], 1, MPI_INT, i, tag, MPI_COMM_WORLD);
              cont = j;
            }
            offset = cont+1;
        }
    } else {
        source = 0;
        for(int i=0; i<num_ints_per_process; i++){
          rc = MPI_Recv(&message[i], 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
        }
        printf("Process %d received:\n", rank);
        for(int i=0; i<num_ints_per_process; i++){
          printf("%d, ",message[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
}
