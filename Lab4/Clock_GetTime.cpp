#include <stdlib.h>
#include <stdio.h>
#include <time.h>

double  SerExpanGregoryLeibniz(int N)
{
    double res = 0, signDef = 1.0;
    for(int i = 0; i < N; ++i)
    {
        res += (4.0 * signDef / (2.0 * (i) + 1.0));
        signDef *= (-1);
    }
    return res;
}

int main(int argc,  char *argv[]) {
    if(argc != 2) return 1;
    
    int N = atoll(argv[1]);

    double res = SerExpanGregoryLeibniz(N);

    printf("%lf", res);
    return 0;
}




