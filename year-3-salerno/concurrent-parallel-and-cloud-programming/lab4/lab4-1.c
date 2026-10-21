#include "mpi.h"
#include <stdio.h>

int main(argc,argv) int argc; char *argv[];
{
    int numtasks, rank, dest, source, rc, count, tag=1;  
    int inmsg, outmsg=3;
    MPI_Status Stat;

    //When a program is ran with MPI all the processes are grouped in what we call a communicator. 
    //You can see a communicator as a box grouping processes together, allowing them to communicate. 
    //Every communication(point to point or collective) is linked to a communicator, allowing the communication to reach different processes. 
    //The default communicator is called MPI_COMM_WORLD where every processes can comunicate wich each of the others processes. 
    //The comunicator has a size(numtasks) and eacc process has an id(rank)
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        dest = 1;
        source = 1;
        rc = MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        rc = MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
    } else if (rank == 1) {
        dest = 0;
        source = 0;
        rc = MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
        rc = MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
    }

    rc = MPI_Get_count(&Stat, MPI_INT, &count);
    printf("Task %d: Received %d int(s) from task %d with tag %d \n",
           rank, count, Stat.MPI_SOURCE, Stat.MPI_TAG);
    MPI_Finalize();
}
