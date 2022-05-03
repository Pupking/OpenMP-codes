#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int median(int a[], int n){
	qsort(a, n, sizeof(int), cmpfunc);
    return a[n/2];
}
 
int qselect(int k, int a[], int n, int blockSize) 
{
 	int **subarrays, *medians, *left, *right;
 	int i, j, l, m, num, pivot, len_l = 0, len_r = 0;
 
 	if (n == 1) {
  		return a[0];
 	}
 
 	num = ceil(n / (double)blockSize);
 
 	subarrays = (int**)malloc(sizeof(int*)*num);
	#pragma omp parallel for
 	for (i=0;i<num;i++){
  		subarrays[i] = (int*)malloc(sizeof(int)*blockSize);
 	}
 
 	medians = (int*) malloc(sizeof(int*) * num);

 	for (i = 0, j = 0; i < n; i = i + blockSize, j++) 
 	{
  		if (j == num) {
   			for (l = 0; l < n % blockSize; l++) {
    			subarrays[j][l] = a[i + l];
   			}
  		} 
  		else 
  		{
   			for (l = 0; l < blockSize; l++) 
   			{ 
    			subarrays[j][l] = a[i + l];
   			}
  		}
 	}
 	
 	#pragma omp parallel
 	{
 	int med;
 	#pragma omp for
 	for (i = 0; i < num; i++) 
 	{
  		if (n % blockSize != 0 && i == num - 1) {
   			med = median(subarrays[i], n % blockSize);
  		}
  		else
  		{
   			med = median(subarrays[i], blockSize);
  		}
  		
  		#pragma omp critical
  		medians[i] = med;
 	}
 	}
 
 	if (num <= blockSize) {
  		pivot = median(medians, num);
 	} 
 	else 
 	{
  		pivot = qselect(num/2, medians, num, blockSize);
 	}
 	
 	for (i = 0; i < n; i++) {
  		if (a[i] > pivot) {
   			len_r++;
  		} 
  		if (a[i] < pivot) 
  		{
   			len_l++;
  		}
 	}
 
 	left = (int*) malloc(sizeof(int*) * len_l);
 	right = (int*) malloc(sizeof(int*) * len_r);
 
 	l = 0, m = 0;
 	for (i = 0; i < n; i++) 
 	{
  		if (a[i] > pivot)
  		{
   			right[l] = a[i];
   			l++;
  		}
  		 
  		if (a[i] < pivot) 
  		{
   			left[m] = a[i];
   			m++;
  		}
 	}
 	
 	if (len_l == k - 1) {
  		return a[0];
 	}
 	if (len_l > k - 1) 
 	{
  		return qselect(k, left, len_l, blockSize);
 	} 
 	if (len_l < k - 1) 
 	{
  		return qselect(k - len_l - 1, right, len_r, blockSize);
 	}
 	
 	return -1;
}

int main(int argc, char* argv[]) 
{
    int n=10000000;
    int i=0;
    int k;
    int *a;
    double start, stop;
    int numTh = atoi(argv[1]);
    int blockSize = atoi(argv[2]);
    a = (int *)malloc(n*sizeof(int));
    
    printf("Enter Elements\n");
    FILE *fp;
    fp = fopen("file12", "r");
    
    if(fp == NULL) {
        printf("Cannot read the file");
    }
    else
    {
        printf("The file is opened\n");
        for(i = 0; i < n; i++) {
            fscanf(fp, "%d", &a[i]);
        }
    }
    k = n/2;
    
    start = omp_get_wtime();
    omp_set_num_threads(numTh);
    int m = qselect(k, a, n, blockSize);
    stop = omp_get_wtime();
    printf("Time taken: %f\n%d\n", stop-start,m);
    return 0;
}
