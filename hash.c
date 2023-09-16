#include "miracl.h"
#include <stdio.h>
#include <time.h>

#define MIN_TIME 10.0
#define MIN_ITERS 1000

#define FIX
#define H0 0x67452301L
#define H1 0xefcdab89L
#define H2 0x98badcfeL
#define H3 0x10325476L
#define H4 0xc3d2e1f0L

#define K0 0x5a827999L
#define K1 0x6ed9eba1L
#define K2 0x8f1bbcdcL
#define K3 0xca62c1d6L

#define PAD 0x80
#define ZERO 0

/* functions */

#define S(n, x) (((x) << n) | ((x) >> (32 - n)))

#define F0(x, y, z) (z ^ (x & (y ^ z)))
#define F1(x, y, z) (x ^ y ^ z)
#define F2(x, y, z) ((x & y) | (z & (x | y)))
#define F3(x, y, z) (x ^ y ^ z)

static void shs_transform(sha *sh)
{ /* basic transformation step */
    mr_unsign32 a, b, c, d, e, temp;
    int t;
#ifdef FIX
    for (t = 16; t < 80; t++)
        sh->w[t] = S(1, sh->w[t - 3] ^ sh->w[t - 8] ^ sh->w[t - 14] ^ sh->w[t - 16]);
#else
    for (t = 16; t < 80; t++)
        sh->w[t] = sh->w[t - 3] ^ sh->w[t - 8] ^ sh->w[t - 14] ^ sh->w[t - 16];
#endif
    a = sh->h[0];
    b = sh->h[1];
    c = sh->h[2];
    d = sh->h[3];
    e = sh->h[4];
    for (t = 0; t < 20; t++)
    { /* 20 times - mush it up */
        temp = K0 + F0(b, c, d) + S(5, a) + e + sh->w[t];
        e = d;
        d = c;
        c = S(30, b);
        b = a;
        a = temp;
    }
    for (t = 20; t < 40; t++)
    { /* 20 more times - mush it up */
        temp = K1 + F1(b, c, d) + S(5, a) + e + sh->w[t];
        e = d;
        d = c;
        c = S(30, b);
        b = a;
        a = temp;
    }
    for (t = 40; t < 60; t++)
    { /* 20 more times - mush it up */
        temp = K2 + F2(b, c, d) + S(5, a) + e + sh->w[t];
        e = d;
        d = c;
        c = S(30, b);
        b = a;
        a = temp;
    }
    for (t = 60; t < 80; t++)
    { /* 20 more times - mush it up */
        temp = K3 + F3(b, c, d) + S(5, a) + e + sh->w[t];
        e = d;
        d = c;
        c = S(30, b);
        b = a;
        a = temp;
    }
    sh->h[0] += a;
    sh->h[1] += b;
    sh->h[2] += c;
    sh->h[3] += d;
    sh->h[4] += e;
}

void shs_init(sha *sh)
{ /* re-initialise */
    int i;
    for (i = 0; i < 80; i++)
        sh->w[i] = 0L;
    sh->length[0] = sh->length[1] = 0L;
    sh->h[0] = H0;
    sh->h[1] = H1;
    sh->h[2] = H2;
    sh->h[3] = H3;
    sh->h[4] = H4;
}

void shs_process(sha *sh, int byte)
{ /* process the next message byte */
    int cnt;

    cnt = (int)((sh->length[0] / 32) % 16);

    sh->w[cnt] <<= 8;
    sh->w[cnt] |= (mr_unsign32)(byte & 0xFF);

    sh->length[0] += 8;
    if (sh->length[0] == 0L)
    {
        sh->length[1]++;
        sh->length[0] = 0L;
    }
    if ((sh->length[0] % 512) == 0)
        shs_transform(sh);
}

void shs_hash(sha *sh, char hash[20])
{ /* pad message and finish - supply digest */
    int i;
    mr_unsign32 len0, len1;
    len0 = sh->length[0];
    len1 = sh->length[1];
    shs_process(sh, PAD);
    while ((sh->length[0] % 512) != 448)
        shs_process(sh, ZERO);
    sh->w[14] = len1;
    sh->w[15] = len0;
    shs_transform(sh);
    for (i = 0; i < 20; i++)
    { /* convert to bytes */
        hash[i] = (char)((sh->h[i / 4] >> (8 * (3 - i % 4))) & 0xffL);
    }
    shs_init(sh);
}

char test[] = "dsajkhdkashdkjashkdhsakhdkqwbkq";

int main()
{
    char hash[20];
    int i;
    sha sh;
    clock_t start;
    int iterations = 0;
    double elapsed;
    start = clock();
    do
    {
        shs_init(&sh);
        for (i = 0; test[i] != 0; i++)
            shs_process(&sh, test[i]);
        shs_hash(&sh, hash);
        iterations++;
    } while (iterations < MIN_ITERS);
    elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    elapsed = 1000.0 * elapsed / iterations;
    printf("ER - %8d iterations    ", iterations);
    printf(" %8.5lf ms per iteration\n", elapsed);
    for (i = 0; i < 20; i++)
        printf("%02x", (unsigned char)hash[i]);
    printf("\n");
    return 0;
}