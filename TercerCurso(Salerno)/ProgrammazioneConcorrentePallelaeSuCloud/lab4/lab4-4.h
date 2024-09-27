#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    MPI_Comm comm;
    int rank;
    int numtasks;
} MyCollective;

//encapsular la información del comunicador MPI, el rango y el tamaño
//La inicialización dr hce llamando a esta función
void my_collective_init(MyCollective *mc) {
    MPI_Comm_rank(MPI_COMM_WORLD, &(mc->rank));
    MPI_Comm_size(MPI_COMM_WORLD, &(mc->numtasks));
    mc->comm = MPI_COMM_WORLD;
}


//******************************************************************************************************************************
//Operaciones blocantes
//******************************************************************************************************************************

//Input:
//mc: estructura de tipo MyCollective
//message: array de enteros que el procesador root va a distribuir entre los demás procesadores 
//count: tamaño del array message
//root; procesador que distribuye la información, en general el 0
void my_broadcast(MyCollective *mc, int *message, int count, int root) {

  if (mc->rank == root) {
        for (int i = 1; i < mc->numtasks; i++) {
            MPI_Send(message, count, MPI_INT, i, 1, mc->comm);
        }
  }else {
        int source = root;
        //MPI_STATUS_IGNORE informs MPI to not fill an MPI_Status, which saves some time.
        MPI_Recv(message, count, MPI_INT, source, 1, mc->comm, MPI_STATUS_IGNORE);
        printf("Process %d received:\n", mc->rank);
        for(int i=0; i<100; i++){
          printf("%d, ",message[i]);
        }
        printf("\n");
  }

}

void my_gather(MyCollective *mc, int *message, int root){
    if (mc->rank == root) {
        printf("El proceso %d recibe los siguientes numeros: \n", mc->rank);
        for (int i = 1; i < mc->numtasks; i++) {
            MPI_Recv(&message[i], 1, MPI_INT, i, 1, mc->comm, MPI_STATUS_IGNORE);
            printf("%d recibido del proceso: %d, ",message[i], i);
        }
    } else {
        int source = root;
        int data = mc->rank;
        MPI_Send(&data, 1, MPI_INT, 0, 1, mc->comm);
    }
}

void my_scatter(MyCollective *mc, int *message, int root){
    int num_ints_per_process= 100/(mc->numtasks-1);

    if (mc->rank == root) {
        int offset = 0;
        int cont;
        for (int i = 1; i < mc->numtasks; i++) {
            for (int j=offset; j < offset + num_ints_per_process; j++){
              MPI_Send(&message[j], 1, MPI_INT, i, 1, mc->comm);
              cont = j;
            }
            offset = cont+1;
        }
    } else {
        int source = root;
        for(int i=0; i<num_ints_per_process; i++){
          MPI_Recv(&message[i], 1, MPI_INT, source, 1, mc->comm, MPI_STATUS_IGNORE);
        }
        printf("Process %d received:\n", mc->rank);
        for(int i=0; i<num_ints_per_process; i++){
          printf("%d, ",message[i]);
        }
        printf("\n");
    }
}

void my_reduction(MyCollective *mc, int *array, int length){
    
    int size = length;
    
    MPI_Barrier(mc->comm);
    int max=array[0], min=array[0];

    int levels;
    if(size%mc->numtasks==0){
      levels=size/mc->numtasks;
    }else if(mc->rank == mc->numtasks-1){
      levels=size/mc->numtasks + (size%mc->numtasks);
    } else{
      levels=size/mc->numtasks;
    }
    
    MPI_Barrier(mc->comm);
    for(int i=mc->rank*(size/mc->numtasks); i<mc->rank*(size/mc->numtasks)+levels; i++){
        if(max < array[i]) {
            max=array[i];
        }
        if(min > array[i]) {
            min=array[i];  
         } 
    }
  
    MPI_Barrier(mc->comm);
    int minimo, maximo;
    //Comunicación blocante
    if (mc->rank == 0) {
            maximo = max;
            minimo = min;
            
            for (int i = 1; i < mc->numtasks; i++) {
                
                MPI_Recv(&max, 1, MPI_INT, i, 1, mc->comm, MPI_STATUS_IGNORE);
                if(maximo < max) {
                    maximo=max; 
                } 
                MPI_Recv(&min, 1, MPI_INT, i, 1, mc->comm, MPI_STATUS_IGNORE);
                if(minimo > min){
                    minimo=min;
                }
            }
          //printf("rank: %d, max: %d\n", mc->rank,maximo);
    } else {
            MPI_Send(&max, 1, MPI_INT, 0, 1, mc->comm);
            MPI_Send(&min, 1, MPI_INT, 0, 1, mc->comm);
    }

    if(mc->rank==0){
        printf("El minimo es: %d y el maximo es: %d\n", minimo, maximo);
    }

}


//******************************************************************************************************************************
//Operaciones no blocantes
//******************************************************************************************************************************

void my_broadcast_non_blocking(MyCollective *mc, int *message, int count, int root, MPI_Request *request, MPI_Status *status) {
 
 //El request te dice si la operacion de envio o de recibimiento se ha hecho
 //El wait espera a que request se marque como hecho, IMPORTANTE usarlo antes
 //de usar los datos recibidos

  if (mc->rank == root) {
        for (int i = 1; i < mc->numtasks; i++) {
            MPI_Isend(message, count, MPI_INT, i, 1, mc->comm, request);  //Un puntero no hay que pasarlo por referencia
        }
  }else {
        int source = root;
        //MPI_STATUS_IGNORE informs MPI to not fill an MPI_Status, which saves some time.
        MPI_Irecv(message, count, MPI_INT, source, 1, mc->comm, request);
        MPI_Wait(request, status);
        printf("Process %d received:\n", mc->rank);
        for(int i=0; i<count; i++){
          printf("%d, ",message[i]);
        }
        printf("\n");
  }

}

void my_gather_non_blocking(MyCollective *mc, int *message, int root, MPI_Request *request, MPI_Status *status){
    if (mc->rank == root) {
        printf("El proceso %d recibe los siguientes numeros: \n", mc->rank);
        for (int i = 1; i < mc->numtasks; i++) {
            MPI_Irecv(&message[i], 1, MPI_INT, i, 1, mc->comm, request);
            printf("%d recibido del proceso: %d, ",message[i], i);
        }
    } else {
        int source = root;
        int data = mc->rank;
        MPI_Isend(&data, 1, MPI_INT, 0, 1, mc->comm, request);
    }
}

void my_scatter_non_blocking(MyCollective *mc, int *message, int root, MPI_Request *request, MPI_Status *status){
    int num_ints_per_process= 100/(mc->numtasks-1);

    if (mc->rank == root) {
        int offset = 0;
        int cont;
        for (int i = 1; i < mc->numtasks; i++) {
            for (int j=offset; j < offset + num_ints_per_process; j++){
              MPI_Isend(&message[j], 1, MPI_INT, i, 1, mc->comm, request);
              cont = j;
            }
            offset = cont+1;
        }
    } else {
        int source = root;
        for(int i=0; i<num_ints_per_process; i++){
          MPI_Irecv(&message[i], 1, MPI_INT, source, 1, mc->comm, request);
        }
        MPI_Wait(request, status);
        printf("Process %d received:\n", mc->rank);
        for(int i=0; i<num_ints_per_process; i++){
          printf("%d, ",message[i]);
        }
        printf("\n");
    }
}


void my_reduction_non_blocking(MyCollective *mc, int *array, int length, MPI_Request *request, MPI_Status *status){
    
    int size = length;
    
    MPI_Barrier(mc->comm);
    int max=array[0], min=array[0];

    int levels;
    if(size%mc->numtasks==0){
      levels=size/mc->numtasks;
    }else if(mc->rank == mc->numtasks-1){
      levels=size/mc->numtasks + (size%mc->numtasks);
    } else{
      levels=size/mc->numtasks;
    }
    
    MPI_Barrier(mc->comm);
    for(int i=mc->rank*(size/mc->numtasks); i<mc->rank*(size/mc->numtasks)+levels; i++){
        if(max < array[i]) {
            max=array[i];
        }
        if(min > array[i]) {
            min=array[i];  
         } 
    }
  
    MPI_Barrier(mc->comm);
    int minimo, maximo;
    //Comunicación blocante
    if (mc->rank == 0) {
            maximo = max;
            minimo = min;
            
            for (int i = 1; i < mc->numtasks; i++) {
                
                MPI_Irecv(&max, 1, MPI_INT, i, 1, mc->comm, request);
                MPI_Wait(request, status);
                if(maximo < max) {
                    maximo=max; 
                } 
                MPI_Irecv(&min, 1, MPI_INT, i, 1, mc->comm, request);
                MPI_Wait(request, status);
                if(minimo > min){
                    minimo=min;
                }
            }
          //printf("rank: %d, max: %d\n", mc->rank,maximo);
    } else {
            MPI_Isend(&max, 1, MPI_INT, 0, 1, mc->comm, request);
            MPI_Isend(&min, 1, MPI_INT, 0, 1, mc->comm, request);
    }

    if(mc->rank==0){
        printf("El minimo es: %d y el maximo es: %d\n", minimo, maximo);
    }

}


