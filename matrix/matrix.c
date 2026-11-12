#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20
#define NUM_THREADS 10

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 



void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
    int threadId = *(int*) args;
    for(int i = threadId; i<MAX; i += NUM_THREADS){
      for(int j = 0 ; j< MAX; j++){
        matSumResult[i][j] = matA[i][j] +matB[i][j];
      }
    }
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
    int threadId = *(int*) args;
    for(int i = threadId; i<MAX; i += NUM_THREADS){
      for(int j = 0 ; j< MAX; j++){
        matDiffResult[i][j] = matA[i][j] -matB[i][j];
      }
    }
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
    int threadId = *(int*) args;
    for(int i = threadId; i<MAX; i += NUM_THREADS){
      for(int j = 0 ; j< MAX; j++){
        int result = 0 ;
        for(int k = 0; k<MAX; k++){
            result+= matA[i][k] *matB[k][j];
        }
        matProductResult[i][j] = result;
      }
    }
    return NULL;
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?

void* ComputeALL(void * args){
  computeSum(args);
  computeDiff(args);
  computeProduct(args);

  return NULL;
}
int main(int argc, char* argv[]) {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    


    // 1. Fill the matrices (matA and matB) with random values.
      fillMatrix(matA);
      fillMatrix(matB);

    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
      pthread_t threads[NUM_THREADS];
      int threadIds[NUM_THREADS];


      for (int i = 0; i<NUM_THREADS; i++){
        threadIds[i] = i;
        if(pthread_create(
          &threads[i],
          NULL,
          ComputeALL,
          &threadIds[i])!= 0
        ) {
          fprintf(stderr, "thread creation failed.\n");
          for(int j = 0 ; j<i ; j++){
            pthread_join(threads[j], NULL);

          }
          return 1;
        }
      }

      for (int i = 0; i< NUM_THREADS; i++){
        pthread_join(threads[i],NULL);

      }
    
    // 5. Wait for all threads to finish.
    
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
  
}