#include <stdio.h>
#include <omp.h>

FILE *fptr0;

int main(int argc, char const *argv[]) {
	fptr0=fopen("Matrix.txt","w");
	double t1, t2, totalT;
	t1 = omp_get_wtime();
	int n = 100;
	float mat[n][n];
	float a[n][n];
	float b[n][n];
	float num = 1.0;
	int j; int i; int k;
	for (int i = 0; i < n-1; i++)
	for (int j = 0; j < n-1; j++)
	a[i][j] = j;

	for (int i = 0; i < n-1; i++)
	for (int j = 0; j < n-1; j++)
	b[i][j] = j;

	for(int j = 0; j < n-1; j+=2){
		for(int i = 0; i < n-1; i++){
			mat[i][j] = 0;
			for (int k = 0; k < n-1; k++)
			mat[i][j] = mat[i][j] + a[i][k]*b[k][j];
		}
		for(int i = 0; i < n-1; i++){
			j++; mat[i][j] = 0;
			for (int k = 0; k < n-1; k++)
			mat[i][j] = mat[i][j] + a[i][k]*b[k][j];
		}
	}
	t2 = omp_get_wtime();
	totalT = t2-t1;
	printf("Tiempo: (%f) segundos \n", totalT);
	fprintf(fptr0,"Tiempo: (%f) segundos\n", totalT);

	for(i = 0; i < 100; i++){
		for (size_t j = 0; j < 100; j++) {
			fprintf(fptr0,"[%d,%d] = %.f\n", i, j, mat[i][j]);
		}
	}

	fclose(fptr0);
}

//Unrolled and Jammed
/*
for(int j = 0; j < n-1; j+=2){
	for(int i = 0; i < n-1; i++){
		mat[i][j] = 0;
		mat[i][j+1] = 0;
		for(int k = 0; k < n-1; k++){
			mat[i][j] = mat[i][j] + a[i][k]*b[k][j];
			mat[i][j] = mat[i][j] + a[i][k]*b[k][j+1];
		}
	}
}
*/
