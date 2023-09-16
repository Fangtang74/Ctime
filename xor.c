#include <stdio.h>
#include <time.h>

#define MIN_TIME 10.0
#define MIN_ITERS 1000

// 定义一个异或函数，参数为两个整数，返回它们的异或结果
int xor (int a, int b) {
    // 定义一个变量result，用来存储异或结果
    int result = 0;
    // 定义一个变量mask，用来表示当前位的值，初始为1
    int mask = 1;
    // 遍历两个整数的每一位
    for (int i = 0; i < 32; i++)
    {
        // 获取两个整数的当前位的值
        int bit_a = a & mask;
        int bit_b = b & mask;
        // 如果两个位不同，就将result的当前位设为1
        if (bit_a != bit_b)
        {
            result = result | mask;
        }
        // 将mask左移一位，准备比较下一位
        mask = mask << 1;
    }
    // 返回异或结果
    return result;
}

    int main()
{
    clock_t start;
    int iterations = 0;
    double elapsed;
    // 定义两个160bit结构体
    int a = 867868; // 二进制表示为00000000000000000000000000001010
    int b = 213123; // 二进制表示为00000000000000000000000000001111
    // 调用异或函数，传入两个结构体的指针，得到它们的异或结果
    start = clock();
    do
    {
        int c = xor(a, b); // 异或结果为5，二进制表示为00000000000000000000000000000101
        iterations++;
    } while (iterations < MIN_ITERS);
    elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    elapsed = 1000.0 * elapsed / iterations;
    printf("ER - %8d iterations    ", iterations);
    printf(" %8.5lf ms per iteration\n", elapsed);
    return 0;
}