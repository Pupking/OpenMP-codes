#include <stdio.h>
#include <limits.h>
#include <stdlib.h> 
#include <omp.h>

int minDistance(int dist[], 
                int sptSet[], int V)
{
       
    int min = INT_MAX, min_index;
   
    for (int v = 0; v < V; v++)
        if (sptSet[v] == -1 &&
                   dist[v] <= min)
            min = dist[v], min_index = v;
   
    return min_index;
}
   
void printPath(int parent[], int j)
{
       
    if (parent[j] == - 1)
        return;
    printf("%d ", j);
}
   
int printSolution(int dist[], int n, 
                      int parent[],int  src)
{
    int V = n;
    printf("Vertex\t Distance\tPath");
    for (int i = 1; i < V; i++)
    {
        printf("\n%d -> %d \t\t %d\t\t%d ",
                      src, i, dist[i], src);
        printPath(parent, i);
    }
}
   
void dijkstra(int V, int** graph, int src, int * dist, int *parent)
{
       
    int sptSet[V];
   
    for (int i = 0; i < V; i++)
    {
        parent[0] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = -1;
    }
   
    dist[src] = 0;
   
    for (int count = 0; count < V - 1; count++)
    {
        int u = minDistance(dist, sptSet, V);
   
        sptSet[u] = 1;
   
        for (int v = 0; v < V; v++)
   
            if (!(sptSet[v] == 1) && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v])
            {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            } 
    }
   
}
   
int main(int argc, char * argv[])
{
    int V = 1000;
    int** grph;
    int thds = atoi(argv[1]);
    double start, stop;
    grph = (int **)malloc(V*sizeof(int*));
    for(int i = 0; i < V; i++)
    {
        grph[i] = (int*)malloc(V*sizeof(int));
    }
//    srand(time(NULL));
    for(int i = 0; i < V; i++)
    {
        for(int j = 0; j <= i; j++)
        {
            grph[i][j] = rand()%10;
            grph[j][i] = grph[i][j];
            grph[i][i] = 0;
        }
    }


/*    for(int i = 0; i < V; i++)
    {
        for(int j = 0; j < V; j++)
        {
            printf("%d ",grph[i][j]);
        }
        printf("\n");
    }
*/  
    start = omp_get_wtime();
    omp_set_num_threads(thds);
    #pragma omp parallel 
    {
        int *dist;
        dist = (int *)malloc(V*sizeof(int));
        int *parent;
        parent = (int *)malloc(V*sizeof(int));
        #pragma omp for
        for(int i = 0; i < V; i++)
        {
            dijkstra(V, grph, i, dist, parent);
        }
    }
    stop = omp_get_wtime();
    printf("Time Taken: %lf\n", stop - start);
    return 0;

}
