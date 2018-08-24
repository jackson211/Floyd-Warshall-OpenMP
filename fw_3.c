#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

//Define the number of nodes in the graph
#define MAX 1200
#define NOT_CONNECTED -1
//Define minimum function that will be used later on to calcualte minimum values between two numbers
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


//Initialize all distances to zero
int dist[MAX][MAX];
int nodesCount;

int main(int argc, char *argv[])
{
  if(argc!=2){
      printf("The path to the input file is not specified as a parameter.\n");
      return -1;
  }
  FILE *in_file  = fopen(argv[1], "r");
  if (in_file  == NULL)
  {
      printf("Can't open file for reading.\n");
      return -1;
  }
  int i, j;
  //Initialization
  for (i=0; i<MAX; ++i){
      for (j=0; j<MAX; ++j){
          dist[i][j]=NOT_CONNECTED;
      }
      dist[i][i]=0;
  }

  //Scan input
  fscanf(in_file,"%d", &nodesCount);
  int a, b, c;
  while(fscanf(in_file,"%d %d %d", &a, &b, &c)!= EOF){
      if ( a > nodesCount || b > nodesCount){
          printf("Vertex index out of boundary.");
          return -1;
      }
      dist[a][b]=c;
  }

  int nthreads;
  int src, dst, middle;
  //Start time
  double start_time = omp_get_wtime();

  for (middle = 0; middle < MAX; middle++)
  {
    int * dm=dist[middle];
    for (src = 0; src < MAX; src++)
    {
      int * ds=dist[src];
      for (dst = 0; dst < MAX; dst++)
      {
        ds[dst]=min(ds[dst],ds[middle]+dm[dst]);
      }
    }
  }


  for(nthreads=1; nthreads <= 8; nthreads++) {
    //Define different number of threads
    omp_set_num_threads(nthreads);

    // Define iterator to iterate over distance matrix
    //Define time variable to record start time for execution of program

    /* Taking a node as mediator
    check if indirect distance between source and distance via mediator
    is less than direct distance between them */
    #pragma omp parallel shared(dist)
    for (middle = 0; middle < MAX; middle++)
    {
      int * dm=dist[middle];
      #pragma omp parallel for private(src, dst) schedule(dynamic)
      for (src = 0; src < MAX; src++)
      {
        int * ds=dist[src];
        for (dst = 0; dst < MAX; dst++)
        {
          ds[dst]=min(ds[dst],ds[middle]+dm[dst]);
        }
      }
    }

    //Total time
    double time = omp_get_wtime() - start_time;
    printf("Total time for thread %d (in sec):%.2f\n", nthreads, time);
  }
  return 0;

}
