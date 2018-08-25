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

    int i, j, k, diameter=-1;
    double timeBegin, timeEnd;
    timeBegin = omp_get_wtime();
    
    for (k=1;k<=nodesCount;++k){
        #pragma omp parallel for private(i, j) shared(distance) 
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
    for (i=1;i<=nodesCount;++i){
       for (j=1;j<=nodesCount;++j){
           if (diameter<dist[i][j]){
               diameter=dist[i][j];
           }
       }
    }
    timeEnd = omp_get_wtime();
    printf("%d %.16g\n", diameter, (timeEnd-timeBegin));

    return 0;
}
