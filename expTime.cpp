#include <iostream>
#define MR_PAIRING_BN
#define AES_SECURITY 128
#include "pairing_3.h"
#include "miracl.h"
#include "bn_pair.cpp"
#include "zzn12a.cpp"
#include "ecn2.cpp"
#include "zzn4.cpp"
#include "zzn2.cpp"
#include "big.cpp"
#include "zzn.cpp"
#include "ecn.cpp"
#include "mrshs.c"
#include <sys/time.h>
#include <string>
#include <vector>
#include <random>
#include <cmath>

// 生成随机字符串
string generate_random_string(size_t length)
{
    const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"; // 可以根据需要包含更多字符
    string random_string;
    size_t charset_size = sizeof(charset) - 1;
    // 生成随机字符串
    while (length--)
    {
        size_t index = rand() % charset_size; // 从字符集中随机选择一个字符
        random_string += charset[index];      // 将选择的字符添加到字符串的末尾
    }
    return random_string;
}

void mapToPoint()
{
    struct timeval start, end;
    long t;
    PFC pfc(AES_SECURITY); // initialise pairing-friendly curve

    G2 Q, V;
    G1 S, R;
    GT result;
    Big s, X;
    long seed = 123456;
    irand((long)seed);

    double sum = 0;
    for (int i = 0; i < 1000; i++)
    {
        gettimeofday(&start, NULL);
        pfc.hash_and_map(R, (char *)"Test Message to sign");
        gettimeofday(&end, NULL);
        double t = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        sum += t;
    }
    sum = sum * 0.000001;
    cout << "==========MapToPoint Hash Time==========" << endl;
    cout << endl;
    cout << "MapToPoint hash runs in ................." << sum << " ms" << endl;
    cout << endl;
}

static Big Hash(const string &message)
{
    char s[20];
    Big num;
    sha sh;
    shs_init(&sh);
    for (auto &ele : message)
    {
        shs_process(&sh, ele);
    }
    shs_hash(&sh, s);
    num = from_binary(20, s);
    return num;
}

void oneWayHash()
{
    double sum = 0, t = 0;
    struct timeval start, end;
    srand(time(NULL));
    PFC pfc(AES_SECURITY);
    for (int i = 0; i < 1000; i++)
    {
        string my_random_string = generate_random_string(64); // 生成一个长度为10的随机字符串
        gettimeofday(&start, NULL);
        Big test = Hash(my_random_string);
        gettimeofday(&end, NULL);
        double t = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        sum += t;
    }
    sum = sum * 0.000001;
    cout << "==========OneWay Hash Time==========" << endl;
    cout << endl;
    cout << "OneWay hash runs in ................." << sum << " ms" << endl;
    cout << endl;
}

double LagrangeInterpolation(const std::vector<double> &x, const std::vector<double> &y, double x0)
{
    double result = 0.0;
    int n = x.size();
    for (int i = 0; i < n; i++)
    {
        double term = y[i];
        for (int j = 0; j < n; j++)
        {
            if (j != i)
            {
                term *= (x0 - x[j]) / (x[i] - x[j]);
            }
        }
        result += term;
    }
    return result;
}

void timeLag()
{
    double sum = 0, t = 0;
    struct timeval start, end;
    std::vector<double> x = {
        11870, 14035, 17366, 20437, 21216, 25016, 30484, 37456, 39204, 39483, 45039, 45335, 46044, 48081, 56822, 57395, 61081, 63913, 65901, 66794};
    std::vector<double> y = {
        51399, 57515, 70988, 75711, 102694, 204614, 231060, 232313, 275067, 275222, 279633, 329188, 330629, 481365, 516220, 564531, 589299, 602779, 636289, 655961};
    // 用于生成随机数的设备和引擎
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(11870, 66794);
    for (int i = 0; i < 1000; i++)
    {
        double x0 = dis(gen);
        double x1 = dis(gen);
        gettimeofday(&start, NULL);
        double y0 = LagrangeInterpolation(x, y, x0);
        double y1 = LagrangeInterpolation(x, y, x1);
        gettimeofday(&end, NULL);
        double t = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        sum += t;
    }
    sum = sum * 0.000001;
    cout << "==========Lagrange Interpolation Time==========" << endl;
    cout << endl;
    cout << "Lagrange interpolation runs in ................." << sum << " ms" << endl;
    cout << endl;
}

double chebyshevPolynomial(int n, double x)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return x;
    return 2 * x * chebyshevPolynomial(n - 1, x) - chebyshevPolynomial(n - 2, x);
}

void timeCheby()
{
    const double x = 0.5; // x的值
    int n = 25;           // 选择较小的n以避免递归深度太大
    double sum = 0;
    struct timeval start, end;
    for (int i = 0; i < 1000; i++)
    {
        gettimeofday(&start, NULL);
        double result = chebyshevPolynomial(n, x);
        gettimeofday(&end, NULL);
        double t = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        sum += t;
    }
    sum = sum * 0.000001;
    cout << "==========Chebyshev Chaotic Map Time==========" << endl;
    cout << endl;
    cout << "Chebyshev chaotic map runs in ................." << sum << " ms" << endl;
    cout << endl;
}

int main()
{
    // execute the above functions
    mapToPoint();
    oneWayHash();
    timeLag();
    timeCheby();
    return 0;
}