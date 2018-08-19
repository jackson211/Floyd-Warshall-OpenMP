#include <stdio.h>
#include <omp.h>

#define MAX 10000
#define NOT_CONNECTED -1

int dist[MAX][MAX];

void Initialize() {
    #pragma omp parallel for shared(dist)
    for (int i=0; i<MAX; ++i){
        for (int j=0; j<MAX; ++j){
            dist[i][j]=NOT_CONNECTED;
        }
        dist[i][i]=0;
    }
}

void floyd_warshall() {
  int i, j, k;

  for (k = 0; k < n; ++k)
      #pragma omp parallel for private(i,j)
      for (i = 0; i < n; ++i)
          for (j = 0; j < n; ++j)
              if ((dist[i][k] * dist[k][j] != 0) && (i != j))
                  if ((dist[i][k] + dist[k][j] < dist[i][j]) || (dist[i][j] == 0))
                      dist[i][j] = dist[i][k] + dist[k][j];
}

int diameter() {
  int diameter=-1;

  //look for the most distant pair
  for (int i=1;i<=nodesCount;++i){
      for (int j=1;j<=nodesCount;++j){
          if (diameter<dist[i][j]){
              diameter=dist[i][j];
              printf("%d-%d-%d\n", i, diameter, j);
          }
      }
  }
  return diameter
}

int main(int argc, char *argv[]) {
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
        dist[a][b]=c;
    }

    double startTime = omp_get_wtime();
    floyd_warshall();
    printf("diameter %d\n", diameter());
    double stopTime = omp_get_wtime();

    printf("time %f\n", stop-start);

    return 0;
}
