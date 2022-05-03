#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>


void merge(int a[], int p, int q, int r) {
    int i,j,k;
    int n1 = q - p + 1; 
    int n2 = r - q;

    int *L, *R;
    L = (int *)malloc(n1*sizeof(int));
    R = (int *)malloc(n2*sizeof(int));

    for(i=0;i<n1;i++) {
        L[i] = a[i+p];
    }
    for(j=0;j<n2;j++) {
        R[j] = a[j+q+1];
    }

    i=0;
    j=0;
    k=p;
    while(i<n1 && j<n2) {
        if(L[i] <= R[j]) {
            a[k] = L[i];
            i++;
        } else {
            a[k] = R[j];
            j++;
        }
        k++;
    }

    while(i<n1) {
        a[k] = L[i];
        i++;
        k++;
    }

    while(j<n2) {
        a[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int a[], int l, int r) {
    if(l<r) {
        int m = l + (r - l)/2;

        #pragma omp parallel
        {
            #pragma omp sections
            {
                #pragma omp section
                {
                    mergeSort(a, l,m);
                }
                #pragma omp section
                {
                    mergeSort(a,m+1,r);
                }
            }
        }

        merge(a, l, m, r);
    }
}

 int main(int argc, char* argv[]) {
    FILE *fp;
    int numTh = atoi(argv[1]);
    int *a;
    int n = 10000000;
    int i;
    double start, stop;
    a = (int *)malloc(n*sizeof(int));

    fp = fopen("file1", "r");

    if(fp == NULL) {
        printf("Cannot read the file");
    } else {
        printf("The file is opened\n");

        for(i = 0; i < n; i++) {
            fscanf(fp, "%d", &a[i]);
        }
    }
    start = omp_get_wtime();
    omp_set_num_threads(numTh);
    
    mergeSort(a, 0, n - 1);
    stop = omp_get_wtime();
    printf("Time taken: %f\n", stop-start);
    
    for(int i = 0; i < n-1; i++)
    {
    	if(a[i] > a[i+1])
    	{
    		printf("FAIL");
    	}
    }
}
