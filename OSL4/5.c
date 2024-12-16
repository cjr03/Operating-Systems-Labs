#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREADS 10000000

pthread_t threads[MAX_THREADS];
double **matrixA, **matrixB, **matrixC; //A, B, C matrices
int N, M, L;

//initialize matrix with random values
void initializeMatrix(int r,int c, double ***matrix){
	*matrix = (double **)malloc(r * sizeof(double *));
	for (int i = 0; i < r*c; i++) (*matrix)[i] = (double *)malloc(c * sizeof(double));
}

void printMatrix(int r,int c, double **matrix){
	for (int i = 0; i < r; i++){
		for (int j = 0; j < c; j++) printf("%3.0f ", matrix[i][j]);
		printf("\n");
	}
}

//thread multiply function
void *multiplySpot(void* arg) {
	int m = (size_t) arg;
	int i = m / L;
	int j = m % L;
	double temp = 0;
	for (int k = 0; k < M; k++) temp += matrixA[i][k] * matrixB[k][j];
	matrixC[i][j] = temp;
	pthread_exit(0);
}

//read N, M, and L as command-line arguments
int main(int argc, char *argv[]) {
	N = atoi(argv[1]), M = atoi(argv[2]), L = atoi(argv[3]);
	initializeMatrix(N, M, &matrixA);
	initializeMatrix(M, L, &matrixB);
	initializeMatrix(N, L, &matrixC);
	srand(time(NULL));
	for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) matrixA[i][j] = rand() % 12;
	srand(time(NULL));
	for (int i = 0; i < M; i++) for (int j = 0; j < L; j++) matrixB[i][j] = rand() % 12;
	int threadc = 0;
	for (int i = 0; i < N; i++) for (int j = 0; j < L; j++) pthread_create(&threads[threadc++], NULL, multiplySpot, (void *)(size_t)(i*L+j));
	for (int i = 0; i < N; i++) for (int j = 0; j < L; j++) pthread_join(threads[i*L+j], NULL);
	printf("Matrix A\n________\n");
        printMatrix(N, M, matrixA);
	printf("Matrix B\n________\n");
        printMatrix(M, L, matrixB);
	printf("Matrix C\n________\n");
	printMatrix(N, L, matrixC);
	for (int i = 0; i < N; i++) {
        	free(matrixA[i]);
        	free(matrixC[i]);
    	}
    	for (int i = 0; i < M; i++) free(matrixB[i]);
    	free(matrixA);
    	free(matrixB);
    	free(matrixC);
	return 0;
}
