#include <stdio.h>
#include "miracl.h"
#include <time.h>
#include <stdlib.h>

#define MIN_TIME 10.0
#define MIN_ITERS 1000
int main()
{
    miracl *mip = mirsys(300, 0); // 初始化MIRACL库
    mip->IOBASE = 10;
    clock_t start;
    int iterations = 0;
    double elapsed;
    start = clock();
    big x;
    char *mem1;
    mem1 = (char *)memalloc(1);
    x = mirvar_mem(mem1, 0);
    do
    {
        bigbits(16, x);
        iterations++;
    } while (iterations < MIN_ITERS);
    elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    elapsed = 1000.0 * elapsed / iterations;
    printf("ER - %8d iterations    ", iterations);
    printf(" %8.5lf ms per iteration\n", elapsed);
    return 0;
}