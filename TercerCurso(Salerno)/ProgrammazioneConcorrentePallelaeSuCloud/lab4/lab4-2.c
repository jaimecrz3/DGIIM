#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(argc,argv) int argc; char *argv[];
{
    int numtasks, rank, dest, source, rc, count, tag=1;  
    MPI_Status Stat;
    
    char message[100];

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        dest = 1;
        source = 1;
        printf("Introduzca un mensaje: \n");
        fgets(message, sizeof(message), stdin);
        int length_message = strlen(message)-1;
        rc = MPI_Send(&message, length_message, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    } else if (rank == 1) {
        dest = 0;
        source = 0;
        rc = MPI_Recv(&message, sizeof(message), MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
        printf("Process 1 received: %s\n", message);
    }

    MPI_Finalize();
}
