#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "myLibrary.h"

int main (int argc, char *argv[])
{
int numtasks, rank, dest, tag, source, rc, count, max = 100;
int acum = 0;
float avg = 0;
char inmsg, outmsg='x';
MPI_Status Stat;
MPI_Request request = MPI_REQUEST_NULL;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

srand(rank*time(NULL));
int j = 0;
for(int i = 0; i < 11; i++){
    
	while(acum < max){
		if(i == 10){
			i = 11;
			acum = max;
		}
			
		MPI_Bcast(&i,1, MPI_INT, j, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        
		if(j == 0 && rank == 0){
			avg +=1;
		}	
			
		if(rank == j){
		    	acum += rand()%101;
			MPI_Isend(&acum, 1, MPI_INT, (rank + 1) % numtasks, 0, MPI_COMM_WORLD, &request);
		}
		else if(rank == (j + 1) % numtasks)
			MPI_Irecv(&acum, 1, MPI_INT, (rank + numtasks - 1) % numtasks, 0, MPI_COMM_WORLD, &request);

		// Wait for completion of non-blocking sends and receives
		MPI_Wait(&request, &Stat);
		
		j = (j+1)%numtasks;
	}
acum = 0;
}

if(rank == 0)
	printf("Average round = %f\n", avg/10);

MPI_Finalize();
}
