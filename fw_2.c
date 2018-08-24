#include <stdio.h>
#include <omp.h>

#define MAX 10000
#define NOT_CONNECTED -1

int dist[MAX][MAX];
int nodesCount;

void Initialize() {
    int i, j;
    #pragma omp parallel for shared(dist)
    for (i=0; i<MAX; ++i){
        for (j=0; j<MAX; ++j){
            dist[i][j]=NOT_CONNECTED;
        }
        dist[i][i]=0;
    }
}

int floyd_warshall() {
    int i, j, k, diameter=-1;
    #pragma omp parallel for
    for (k=1;k<=nodesCount;++k){
        #pragma omp parallel for private(i, j) schedule(dynamic)
        for (i=1;i<=nodesCount;++i){
            if (dist[i][k]!=NOT_CONNECTED){
                for (j=1;j<=nodesCount;++j){
                    if (dist[k][j]!=NOT_CONNECTED && (dist[i][j]==NOT_CONNECTED || dist[i][k]+dist[k][j]<dist[i][j])){
                        dist[i][j]=dist[i][k]+dist[k][j];
                    }
                }
            }
        }
    }
    #pragma omp parallel for private(i, j)
    for (i=1;i<=nodesCount;++i){
       for (j=1;j<=nodesCount;++j){
           if (diameter<dist[i][j]){
               diameter=dist[i][j];
               printf("%d-%d-%d\n", i, diameter, j);
           }
       }
    }
    return diameter;
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
    printf("diameter %d\n", floyd_warshall());
    double stopTime = omp_get_wtime();

    printf("time %f\n", stopTime-startTime);

    return 0;
}
