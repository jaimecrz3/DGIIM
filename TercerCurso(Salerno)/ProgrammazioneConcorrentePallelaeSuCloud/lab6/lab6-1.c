#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[])
{
int numtasks, rank, dest, tag, source, rc, count;
MPI_Status Stat;
MPI_Request send_request = MPI_REQUEST_NULL;
MPI_Request recv_request = MPI_REQUEST_NULL;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);


int val;

int suma = rank;
MPI_Barrier(MPI_COMM_WORLD);
//printf("Suma procesador %d = %d\n", rank, suma);
for(int i = 0; i < numtasks; i++){

	for(int j = 0; j < numtasks; j++){
		if(rank == j && ((rank + 1 + i) % numtasks)!=rank){
		        val = rank;
			MPI_Isend(&val, 1, MPI_INT, (rank + 1 + i) % numtasks, 0, MPI_COMM_WORLD, &send_request);
		}else if(rank == (j + 1 + i) % numtasks  && ((rank + numtasks - 1 - i) % numtasks) != rank){  //Importante esta comprobacion para que un proceso no pueda recbir de si mismo
		        //Ponemos (j+1+i) porque no tiene porque recbir solo del anterior, sino de cualquiera
			MPI_Irecv(&val, 1, MPI_INT, (rank + numtasks - 1 - i) % numtasks, 0, MPI_COMM_WORLD, &recv_request);
			// Wait for completion of non-blocking sends and receives
		        MPI_Wait(&recv_request, &Stat);
		        suma += val;
		        //if(rank==2)
		        //  printf("rango:%d con i:%d y j:%d, num_recv:%d\n", rank, i, j, (rank + numtasks - 1 - i) % numtasks);
		}
		
		
	}
}

//if(rank == 0){
//	  printf("Suma procesador %d = %d\n", rank, suma);
//}else{
printf("Suma procesador %d = %d\n", rank, suma);
//}

MPI_Finalize();
}
