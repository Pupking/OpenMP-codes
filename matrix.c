#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
// find transpose + unroll 
void divideAndMultiply(int n, int** a, int** b, int** c, int block, int thd)
{
n = 2048;
int bi=0;
int bj=0;
int bk=0;
int i=0;
int j=0;
int k=0;
int blockSize=block;

omp_set_num_threads(thd);

#pragma omp parallel for private(bi,bj,bk,i,j,k) shared(a,b,c) collapse(5)
for(bi=0; bi<n; bi+=blockSize)
    for(bj=0; bj<n; bj+=blockSize)
        for(bk=0; bk<n; bk+=blockSize)
            for(i=0; i<blockSize; i++)
                for(j=0; j<blockSize; j++)
                	#pragma unroll
                    for(k=0; k<blockSize; k++)
                            c[bi+i][bj+j] += a[bi+i][bk+k]*b[bk+k][bj+j];
}
int main(int x,char* arr[])
{
FILE* output;
int size = 2048;
int** A;
int** B;
int** C;
int i=0;
int j=0;
int numTh = atoi(arr[2]);
struct timeval tv1, tv2;
double timeTaken;

if(atoi(arr[1]) != 4 && atoi(arr[1]) != 8 && atoi(arr[1]) != 16 && atoi(arr[1]) != 32 && atoi(arr[1]) != 64){
	printf("Please enter correct input");
}

else{
	
	int blockSize = atoi(arr[1]);

	A=(int **)calloc(sizeof(int *), size);
	B=(int **)calloc(sizeof(int *), size);
	C=(int **)calloc(sizeof(int *), size);
	for(i=0;i<size;i++)
	{
		A[i]=(int *)calloc(sizeof(int), size);
		B[i]=(int *)calloc(sizeof(int), size);
		C[i]=(int *)calloc(sizeof(int), size);
	}
	//initialize the matrices
	for(i=0; i< size; i++)
    {
	for(j=0; j<size; j++)
        {
            A[i][j] = 2;//rand()%10;
            B[i][j] = 2;//rand()%10;
        }
    }
	//multiply matrices
	gettimeofday(&tv1, NULL);
	divideAndMultiply(size,A,B,C,blockSize,numTh);
	gettimeofday(&tv2, NULL);
	timeTaken += (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
	output = fopen("output.txt","w");
    
    for(i=0; i<size; i++)
    {
	for(j=0; j<size; j++)
        {
            fprintf(output,"%d ",C[i][j]);
        }
        fprintf(output,"\n");
    }
    
    for(i=0; i<size; i++)
    {
	for(j=0; j<size; j++)
        {
            if(C[i][j] != 8192)
            {
            	printf("FAIL\n");
            }
        }
    }
printf("Time = %lf \n",timeTaken);
return 0;
}
}
