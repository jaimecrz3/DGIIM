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

srand(time(NULL));

const int N=atoi(argv[1]);
const int M=atoi(argv[2]);
const int iterations = atoi(argv[3]);
int matrix[N][M];
//if the rest != 0, the thread zero take cares of it
int levels=M/numtasks;
if(rank==0){
    //Si el valor es cero esta muerto si es uno esta vivo
    printf("Matriz inicial:\n");
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            int num=rand()%2;
            matrix[i][j]=num;
            printf("%d ", num);
        }
        printf("\n");
    }
    printf("\n\n");

}
  
//We send the matrix to the rest of processes
int matrix_threads[N][levels];
MPI_Scatter(&matrix, N*levels, MPI_INT, &matrix_threads, N*levels, MPI_INT, 0, MPI_COMM_WORLD);
//we need to take care of the part that we didnt send
int matrix_offset[N][M%numtasks];
if(M%numtasks!=0 && rank == numtasks-1){
    for(int f=0; f<N;f++){
        for(int c=levels*rank; c<M;c++){
            matrix_offset[0][c-(levels*rank)] = matrix[f][c];
        }
    }
    MPI_Send(&matrix_offset, N*(M%numtasks), MPI_INT, 0, 7, MPI_COMM_WORLD);
}
  
//MPI_Barrier(MPI_COMM_WORLD);

for(int i = 0; i < iterations; i++){
        
        if(rank==0 && M%numtasks!=0){
            
            //Porque matrix se va a ir modificando
            int matrix_copia[N][(M%numtasks)+1];
            
            //In the first iteration the thread 0 receives the offset from the last thread
            if(i==0){
                  MPI_Recv(&matrix_offset, N*(M%numtasks), MPI_INT, numtasks-1, 7, MPI_COMM_WORLD, &Stat);
                  //printf("Matriz en el paso: %d\n", i);
                  for(int f=0; f<N; f++){
                      for(int c=0; c<=M%numtasks; c++){
                          if(c==0){
                              matrix_copia[f][c]=matrix[f][(levels*(numtasks-1))-1];
                          }
                          matrix_copia[f][c] = matrix_offset[f][c-1];
                      }
                  }
            }else{  //In the rest of iterations its receives just the last column from the las thread
                for(int f=0; f<N; f++){   
                        MPI_Irecv(&val, 1, MPI_INT, 0, 6, MPI_COMM_WORLD, &recv_request);
                        //(M/(numtasks-1))*rank
                        matrix_copia[f][0] = val;
                        //MPI_Wait(&recv_request, &Stat);
               }
               MPI_Wait(&recv_request, &Stat);
            }
            
            //We calclulate the next step
            for(int f=0; f<N; f++){
                for(int c=1; c<=M%numtasks; c++){
                
                    //Obtenemos el numero de vecinos vivos
                    int sum_neighbors=0;
                    if(c+1<=M%numtasks){
                        sum_neighbors+=matrix_copia[f][c+1];
                    }
                    if(c-1>=0){
                        sum_neighbors+=matrix_copia[f][c-1];
                    }
                    if(f+1<N){
                        sum_neighbors+=matrix_copia[f+1][c];
                        if(c-1>=0){
                            sum_neighbors+=matrix_copia[f+1][c-1];
                        }
                        if(c+1<M%numtasks){
                            sum_neighbors+=matrix_copia[f+1][c+1];
                        }
                    }
                    if(f-1>=0){
                        sum_neighbors+=matrix_copia[f-1][c];
                        if(c-1>=0){
                            sum_neighbors+=matrix_copia[f-1][c-1];
                        }
                        if(c+1<M%numtasks){
                            sum_neighbors+=matrix_copia[f-1][c+1];
                        }
                    }
                    
                    //if(i==1 && j==1)
                    
                    //Calculamos el nuevo resultado
                    if(matrix_copia[f][c]==1){
                        if(sum_neighbors < 2){
                            matrix_offset[f][c-1] = 0;
                        }else if(sum_neighbors > 3){
                            matrix_offset[f][c-1] = 0;
                        }
                    }else if(sum_neighbors == 3){
                        matrix_offset[f][c-1] = 1;
                    }
                       
                }
                
            }
            
            //El padre envia la matriz actualizada
            for(int f=0; f<N; f++){   
                    MPI_Isend(&matrix_offset[f][0], 1, MPI_INT, numtasks-1, 3, MPI_COMM_WORLD, &send_request);
           }
            
        }else{
            
            //Porque matrix se va a ir modificando
            int matrix_copia[N][levels+2];
            //In the first iteation we need to add the columns from the original matrix for doing the operations
            if(i==0){
                for(int f=0; f<N; f++){
                    for(int c=0; c<=levels+1; c++){
                        if(c==0 && rank != 0){
                            matrix_copia[f][c]=matrix[f][(levels*(rank-1))-1];
                        }else if((c==levels+1 && rank != numtasks-1) || (rank == numtasks-1 && c==levels+1 && (M%numtasks)!=0)){
                            matrix_copia[f][c]=matrix[f][levels*rank];
                        }else if(c!=0 && c!=levels+1){
                            matrix_copia[f][c] = matrix_threads[f][c-1];
                        }
                    }
                }
            }else{
            
                //In the rest of operations its receives the columns from its neighborgs
                for(int f=0; f<N; f++){
                    for(int c=1; c<=levels; c++){
                        matrix_copia[f][c] = matrix_threads[f][c-1];
                    }
                }
            
                if(numtasks!=1){
                    if(rank==0){
                       for(int f=0; f<N; f++){   
                          MPI_Irecv(&val, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &recv_request);
                          matrix_copia[f][levels+1] = val;
                          //MPI_Wait(&recv_request, &Stat);
                        }
                     MPI_Wait(&recv_request, &Stat);
                  }else if(rank == numtasks-1){ 
                       for(int f=0; f<N; f++){   
                          MPI_Irecv(&val, 1, MPI_INT, rank-1, 2, MPI_COMM_WORLD, &recv_request);
                          matrix_copia[f][0] = val;
                          //MPI_Wait(&recv_request, &Stat);
                     }
                     MPI_Wait(&recv_request, &Stat);
                     if(M%numtasks!=0){
                           for(int f=0; f<N; f++){   
                              MPI_Irecv(&val, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &recv_request);
                              matrix_copia[f][levels+1] = val;
                              //MPI_Wait(&recv_request, &Stat);
                           }
                           MPI_Wait(&recv_request, &Stat);
                     }
                  }else{
                         for(int f=0; f<N; f++){   
                            MPI_Irecv(&val, 1, MPI_INT, rank-1, 5, MPI_COMM_WORLD, &recv_request);
                            matrix_copia[f][0] = val;
                            //MPI_Wait(&recv_request, &Stat);
                        }
                        MPI_Wait(&recv_request, &Stat);
                         for(int f=0; f<N; f++){   
                            MPI_Irecv(&val, 1, MPI_INT, rank+1, 4, MPI_COMM_WORLD, &recv_request);
                            matrix_copia[f][levels+1] = val;
                            //MPI_Wait(&recv_request, &Stat);
                        }
                        MPI_Wait(&recv_request, &Stat);
                  }
                  
                  
              }
            }
                
            
            for(int i=0; i<N; i++){
                    for(int j=1; j<=levels; j++){
                        //Obtenemos el numero de vecinos vivos
                        int sum_neighbors=0;
                        if(rank!=numtasks-1 || j<levels){
                            sum_neighbors+=matrix_copia[i][j+1];
                        }else if(M%numtasks!=0){
                            sum_neighbors+=matrix_copia[i][j+1];
                        }
                        if(rank!=0 || j>1){
                            sum_neighbors+=matrix_copia[i][j-1];
                        }
                        if(i+1<N){
                            sum_neighbors+=matrix_copia[i+1][j];
                            if(rank!=0 || j>1){
                                sum_neighbors+=matrix_copia[i+1][j-1];
                            }
                            if(rank!=numtasks-1 || j<levels){
                                sum_neighbors+=matrix_copia[i+1][j+1];
                            }else if(M%numtasks!=0){
                                sum_neighbors+=matrix_copia[i+1][j+1];
                           }
                        }
                        if(i-1>=0){
                            sum_neighbors+=matrix_copia[i-1][j];
                            if(rank!=0 || j>1){
                                sum_neighbors+=matrix_copia[i-1][j-1];
                            }
                            if(rank!=numtasks-1 || j<levels){
                                sum_neighbors+=matrix_copia[i-1][j+1];
                            }else if(M%numtasks!=0){
                                sum_neighbors+=matrix_copia[i-1][j+1];
                           }
                        }
                        
                        //Calculamos el nuevo resultado
                        if(matrix_copia[i][j]==1){
                            if(sum_neighbors < 2){
                                matrix_threads[i][j-1] = 0;
                            }else if(sum_neighbors > 3){
                                matrix_threads[i][j-1] = 0;
                            }
                        }else if(sum_neighbors == 3){
                            matrix_threads[i][j-1] = 1;
                        }
                    }
                }
                
                
                //Enviar a los vecinos la matriz actualizada
                if(numtasks!=1){
                    if(rank==0){
                       for(int f=0; f<N; f++){   
                          MPI_Isend(&matrix_threads[f][levels-1], 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &send_request);
                        }
                  }else if(rank == numtasks-1){ 
                       for(int f=0; f<N; f++){   
                          MPI_Isend(&matrix_threads[f][0], 1, MPI_INT, rank-1, 2, MPI_COMM_WORLD, &send_request);
                     }
                     if(M%numtasks!=0){
                           for(int f=0; f<N; f++){   
                              MPI_Isend(&matrix_threads[f][levels-1], 1, MPI_INT, 0, 6, MPI_COMM_WORLD, &send_request);
                          }
                      }
                  }else{
                         for(int f=0; f<N; f++){   
                            MPI_Isend(&matrix_threads[f][0], 1, MPI_INT, rank-1, 4, MPI_COMM_WORLD, &send_request);
                        }
                         for(int f=0; f<N; f++){   
                            MPI_Isend(&matrix_threads[f][levels-1], 1, MPI_INT, rank+1, 5, MPI_COMM_WORLD, &send_request);
                        }
                  }
                  
                  
              }
                         
        }//else
        
}

MPI_Gather(&matrix_threads, N*levels, MPI_INT, &matrix, N*levels, MPI_INT, 0, MPI_COMM_WORLD);

if(rank==0){
    
    if(M%numtasks!=0){
        for(int i=0; i<N; i++){
            for(int j=levels*(numtasks-1); j<M; j++){
                  matrix[i][j] = matrix_offset[i][j-(levels*(numtasks-1))];
            }
        }
    }
    
    printf("Matriz final:\n");
    for(int i=0; i<N; i++){
          for(int j=0; j<M; j++){
                printf("%d ", matrix[i][j]);
          }
          printf("\n");
    }
}


MPI_Finalize();
}
