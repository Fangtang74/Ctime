#include <stdio.h>

float lagelangri(float x[], float y[], float xx, int n)
{
    int i, j;
    float *a, yy = 0;
    a = new float[n];
    for (i = 0; i <= n - 1; i++)
    {
        a[i] = y[i];
        for (j = 0; j <= n - 1; j++)
            if (j != i)
                a[i] *= (xx - x[j]) / (x[i] - x[j]);

        yy += a[i];
    }
    delete a;
    return yy;
}

int main()
{
    float x[5] = {-3.0, -1.0, 1.0, 2.0, 3.0};
    float y[5] = {1.0, 1.5, 2.0, 2.0, 1.0};
    float xx1 = -2, xx2 = 0, xx3 = 2.75, yy1, yy2, yy3;
    yy1 = lagelangri(x, y, xx1, 2);
    printf("x1=%-20f,y1=%f\n", xx1, yy1);
    return 0;
}