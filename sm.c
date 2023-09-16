#include "miracl.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* define minimum duration of each timing, and min. number of iterations */
#define MIN_TIME 10.0
#define MIN_ITERS 1000

// 点乘操作函数
double mults(int eb, epoint *g)
{
    big e;
    int iterations = 0;
    clock_t start;
    double elapsed;
    epoint *w, *r;
    char *mem1;
    char *mem2;
    mem1 = (char *)memalloc(1);
    mem2 = (char *)ecp_memalloc(2);

    e = mirvar_mem(mem1, 0);
    w = epoint_init_mem(mem2, 0);
    r = epoint_init_mem(mem2, 1);

    bigbits(eb, e);
    ecurve_mult(e, g, r); /* generate a random point on the curve */
    bigbits(eb, e);
    start = clock();
    do
    {
        ecurve_mult(e, r, w);
        iterations++;
    } while (iterations < MIN_ITERS);
    elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    elapsed = 1000.0 * elapsed / iterations;
    printf("ER - %8d iterations    ", iterations);
    printf(" %8.5lf ms per iteration\n", elapsed);

    memkill(mem1, 1);
    ecp_memkill(mem2, 2);
    return elapsed;
}

int main()
{
    miracl *mip = mirsys(300, 0); // 初始化MIRACL库
    mip->IOBASE = 10;
    int k;
    epoint *g;
    double tr1 = 0;
    big a, b, p, x, y; // 声明大数变量
    a = mirvar(-3);
    x = mirvar(1);
    b = mirvar(0);
    p = mirvar(0);
    y = mirvar(0);
    k = 160;
    cinstr(b, "547961736808018748879088091015409822321903727720");
    cinstr(p, "1243254415344564576487568858887587143562341624873");
    cinstr(y, "1184021062507719516935416374276431034553065993786");
    printf("\n160 bit GF(p) Elliptic Curve....\n");
    ecurve_init(a, b, p, MR_PROJECTIVE);
    g = epoint_init();
    if (!epoint_set(x, y, 0, g))
    {
        printf("This is not a point on the curve!\n");
        exit(0);
    }

    tr1 = mults(k, g);
    return 0;
}