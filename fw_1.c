#include <stdio.h>
#include <omp.h>

#define MAX 10000
#define NOT_CONNECTED -1
#define MAX_THREAD 8

int distance[MAX][MAX];

int nodesCount;

    //#pragma omp parallel
    void Initialize(){
        int i, j;
        for (i=0; i<MAX; ++i){
    	      for (j=0; j<MAX; ++j){
                distance[i][j]=NOT_CONNECTED;
            }
            distance[i][i]=0;
        }
    }

int main(int argc, char** argv){
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

      Initialize();

      fscanf(in_file,"%d", &nodesCount);

      int a, b, c;
      while(fscanf(in_file,"%d %d %d", &a, &b, &c)!= EOF){
          if ( a > nodesCount || b > nodesCount){
              printf("Vertex index out of boundary.");
              return -1;
          }
          distance[a][b]=c;
      }
      double timeBegin = omp_get_wtime(); 
      int i, j, k;
      //Floyd-Warshall
      //#pragma omp parallel shared(distance)
      for (k=1;k<=nodesCount;++k){
	       // printf("Number of threads: %d\n", omp_get_num_threads());
          for (i=1;i<=nodesCount;++i){
 	              if (distance[i][k]!=NOT_CONNECTED){
                  for (j=1;j<=nodesCount;++j){
                      if (distance[k][j]!=NOT_CONNECTED && (distance[i][j]==NOT_CONNECTED || distance[i][k]+distance[k][j]<distance[i][j])){
                          distance[i][j]=distance[i][k]+distance[k][j];
                      }
                  }
              }
          }
      }

      int diameter=-1;

      //look for the most distant pair
      for (i=1;i<=nodesCount;++i){
          for (j=1;j<=nodesCount;++j){
              if (diameter<distance[i][j]){
                  diameter=distance[i][j];
                  printf("%d-%d-%d\n", i, diameter, j);
              }
          }
      }

      double timeEnd = omp_get_wtime();
      printf("OMP Clock: %lf\n", (timeEnd-timeBegin));
      printf("%d\n", diameter);

      return 0;
}
