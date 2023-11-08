/*
 *
 *   Requires: big.cpp ecn.cpp
 */

#include <iostream>
#include <cstring>
#include <fstream>
#include <sys/timeb.h>
#include "ecn.h"
#include <sys/time.h>
#include "big.cpp"

using namespace std;

#ifndef MR_NOFULLWIDTH
Miracl precision(200, 256);
#else
Miracl precision(50, MAXBASE);
#endif

void strip(char *name)
{ /* strip off filename extension */
    int i;
    for (i = 0; name[i] != '\0'; i++)
    {
        if (name[i] != '.')
            continue;
        name[i] = '\0';
        break;
    }
}

int main()
{
    ifstream common("common.ecs"); /* construct file I/O streams */
    ifstream private_key("private.ecs");
    ifstream message;
    ofstream signature;
    char ifname[50], ofname[50];
    ECn G;
    Big a, b, p, q, x, y, h, r, s, d, k;
    long seed;
    int bits;
    miracl *mip = &precision;
    struct timeval start, end;

    /* randomise */
    //    cout << "Enter 9 digit random number seed  = ";
    //    cin >> seed;
    seed = 123456;
    irand((long)seed);

    /* get common data */
    common >> bits;
    mip->IOBASE = 16;
    common >> p >> a >> b >> q >> x >> y;
    mip->IOBASE = 10;

    /* calculate r - this can be done off-line,
       and hence amortized to almost nothing    */
    ecurve(a, b, p, MR_PROJECTIVE);
    G = ECn(x, y);

    double sum = 0;
    for (int i = 0; i < 1000; ++i)
    {
        Big temp;
        r = randbits(256);
        gettimeofday(&start, NULL);
        ECn ID = r * G;
        gettimeofday(&end, NULL);
        double t = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        sum += t;
    }
    sum = sum * 0.000001;
    cout << "==========ECC Scalar Multiplication Time==========" << endl;
    cout << endl;
    cout << "ECC scalar multiplication runs in ................." << sum << " ms" << endl;
    cout << endl;
    sum = 0;
    for (int i = 0; i < 1000; ++i)
    {
        r = rand(q);
        ECn ID1 = r * G;
        r = rand(q);
        ECn ID2 = r * G;
        gettimeofday(&start, NULL);
        ID2 += ID1;
        gettimeofday(&end, NULL);
        double t = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        sum += t;
    }
    sum = sum * 0.000001;
    cout << "==========ECC Point Addition Time==========" << endl;
    cout << endl;
    cout << "ECC point addition runs in ................." << sum << " ms" << endl;
    cout << endl;
    sum = 0;
    for (int i = 0; i < 1000; i++)
    {
        gettimeofday(&start, NULL);
        r = randbits(256);
        gettimeofday(&end, NULL);
        double t = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        sum += t;
    }
    sum = sum * 0.000001;
    cout << "==========Random Number Generation Time==========" << endl;
    cout << endl;
    cout << "Random number generation runs in ................." << sum << " ms" << endl;
    cout << endl;
    return 0;
}
