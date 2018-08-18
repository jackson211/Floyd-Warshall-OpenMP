#include <stdio.h>
#include <omp.h>

#define MAX 10000
#define NOT_CONNECTED -1
#define MAX_THREAD 8

int distance[MAX][MAX];

int nodesCount;

    //#pragma omp parallel
    void Initialize(){
        #pragma omp parallel for shared(distance)
        for (int i=0; i<MAX; ++i){
    	      for (int j=0; j<MAX; ++j){
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

      double timeBegin, timeEnd;
      timeBegin = omp_get_wtime();
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

      //Floyd-Warshall
      omp_set_num_threads(MAX_THREAD);
      #pragma omp parallel shared(distance) 
      for (int k=1;k<=nodesCount;++k){
	  printf("Number of threads: %d\n", omp_get_num_threads());
          for (int i=1;i<=nodesCount;++i){
 	      if (distance[i][k]!=NOT_CONNECTED){
                  for (int j=1;j<=nodesCount;++j){
                      if (distance[k][j]!=NOT_CONNECTED && (distance[i][j]==NOT_CONNECTED || distance[i][k]+distance[k][j]<distance[i][j])){
                          distance[i][j]=distance[i][k]+distance[k][j];
                      }
                  }
              }
          }
      }

      int diameter=-1;

      //look for the most distant pair
      for (int i=1;i<=nodesCount;++i){
          for (int j=1;j<=nodesCount;++j){
              if (diameter<distance[i][j]){
                  diameter=distance[i][j];
                  //printf("%d-%d-%d\n", i, diameter, j);
              }
          }
      }

      timeEnd = omp_get_wtime();
      printf("OMP Clock: %lf\n", (timeEnd-timeBegin));
      printf("%d\n", diameter);

      return 0;
}
