#include <stdio.h>
#include <omp.h>

#define MAX 10000
#define NOT_CONNECTED -1

int distance[MAX][MAX];

int nodesCount;


// void Initialize(){
//     #pragma omp parallel for shared(distance)
//     for (int i=0; i<MAX; ++i){
// 	      for (int j=0; j<MAX; ++j){
//             distance[i][j]=NOT_CONNECTED;
//         }
//         distance[i][i]=0;
//     }
// }

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
      #pragma omp parallel for shared(distance)
      for (int i=0; i<MAX; ++i){
          for (int j=0; j<MAX; ++j){
              distance[i][j]=NOT_CONNECTED;
          }
          distance[i][i]=0;
      }

      int nThreds;
      #pragma omp parallel
      {
        int id = omp_get_thread_num();
        if (id == 0)
        {
          nThreds = omp_get_num_threads();
        }
        printf("thread number is: %d, Total threads is: %d.\n", id, nThreds);
      }

      fscanf(in_file,"%d", &nodesCount);

      int a, b, c;
      while(fscanf(in_file,"%d %d %d", &a, &b, &c)!= EOF){
          if ( a > nodesCount || b > nodesCount){
              printf("Vertex index out of boundary.");
              return -1;
          }
          distance[a][b]=c;
      }

      //#pragma omp parallel for shared(distance, nodesCount)
      //Floyd-Warshall
      for (int k=1;k<=nodesCount;++k){
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
