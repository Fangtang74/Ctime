// // 引入必要的头文件
// #include <iostream>
// #include <chrono>
// #include <openssl/aes.h>

// // 定义AES-CBC加密的函数，参数为明文、密钥、初始向量和密文
// void aes_cbc_encrypt(const unsigned char *plaintext, const unsigned char *key, const unsigned char *iv, unsigned char *ciphertext)
// {
//     // 创建一个AES_KEY结构体，用于存储加密密钥
//     AES_KEY aes_key;
//     // 使用密钥初始化AES_KEY结构体
//     AES_set_encrypt_key(key, 128, &aes_key);
//     // 调用AES_cbc_encrypt函数，进行AES-CBC加密
//     AES_cbc_encrypt(plaintext, ciphertext, 16, &aes_key, const_cast<unsigned char *>(iv), AES_ENCRYPT);
// }

// // 定义一个测试函数，用于执行1000次AES-CBC加密，并返回平均时间
// double test_aes_cbc()
// {
//     // 定义一个明文，长度为16字节
//     const unsigned char plaintext[16] = "Hello, world!";
//     // 定义一个密钥，长度为16字节
//     const unsigned char key[16] = "This is a key!";
//     // 定义一个初始向量，长度为16字节
//     const unsigned char iv[16] = "This is an iv!";
//     // 定义一个密文，长度为16字节
//     unsigned char ciphertext[16];
//     // 定义一个累计时间变量，初始为0
//     double total_time = 0;
//     // 定义一个循环变量，用于控制执行次数
//     int i;
//     // 循环执行1000次AES-CBC加密
//     for (i = 0; i < 1000; i++)
//     {
//         // 获取当前时间，作为加密开始时间
//         auto start = std::chrono::high_resolution_clock::now();
//         // 调用AES-CBC加密函数
//         aes_cbc_encrypt(plaintext, key, iv, ciphertext);
//         // 获取当前时间，作为加密结束时间
//         auto end = std::chrono::high_resolution_clock::now();
//         // 计算加密所用时间，单位为微秒
//         auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//         // 将加密所用时间累加到总时间变量
//         total_time += duration.count();
//     }
//     // 计算平均时间，单位为微秒
//     double average_time = total_time / 1000;
//     // 返回平均时间
//     return average_time;
// }

// // 定义主函数，用于测试
// int main()
// {
//     // 调用测试函数，获取平均时间
//     double average_time = test_aes_cbc();
//     // 输出平均时间
//     std::cout << "The average time of AES-CBC encryption is " << average_time << " microseconds." << std::endl;
//     // 返回0，表示程序正常结束
//     return 0;
// }
// 引入必要的头文件
#include <iostream>
#include <chrono>
#include <openssl/aes.h>
#include <sys/time.h>

// 定义AES-CBC加密的函数，参数为明文、密钥、初始向量和密文
void aes_cbc_encrypt(const unsigned char *plaintext, const unsigned char *key, const unsigned char *iv, unsigned char *ciphertext)
{
    // 创建一个AES_KEY结构体，用于存储加密密钥
    AES_KEY aes_key;
    // 使用密钥初始化AES_KEY结构体
    AES_set_encrypt_key(key, 128, &aes_key);
    // 调用AES_cbc_encrypt函数，进行AES-CBC加密
    AES_cbc_encrypt(plaintext, ciphertext, 16, &aes_key, const_cast<unsigned char *>(iv), AES_ENCRYPT);
}

// 定义AES-CBC解密的函数，参数为密文、密钥、初始向量和明文
void aes_cbc_decrypt(const unsigned char *ciphertext, const unsigned char *key, const unsigned char *iv, unsigned char *plaintext)
{
    // 创建一个AES_KEY结构体，用于存储解密密钥
    AES_KEY aes_key;
    // 使用密钥初始化AES_KEY结构体
    AES_set_decrypt_key(key, 128, &aes_key);
    // 调用AES_cbc_encrypt函数，进行AES-CBC解密
    AES_cbc_encrypt(ciphertext, plaintext, 16, &aes_key, const_cast<unsigned char *>(iv), AES_DECRYPT);
}

// 定义一个测试函数，用于执行1000次AES-CBC加密和解密，并返回平均时间
double *test_aes_cbc()
{
    double sum = 0, t = 0, sum1 = 0, sum2 = 0;
    struct timeval start, end;
    // 定义一个明文，长度为16字节
    const unsigned char plaintext[16] = "Hello, world!";
    // 定义一个密钥，长度为16字节
    const unsigned char key[16] = "This is a key!";
    // 定义一个初始向量，长度为16字节
    const unsigned char iv[16] = "This is an iv!";
    // 定义一个密文，长度为16字节
    unsigned char ciphertext[16];
    // 定义一个解密后的明文，长度为16字节
    unsigned char decrypted[16];
    // 定义一个累计时间变量，初始为0
    double total_time = 0;
    // 定义一个循环变量，用于控制执行次数
    int i;
    // 定义一个静态数组，用于存储加密和解密的平均时间
    static double average_time[2];
    // 循环执行1000次AES-CBC加密
    for (i = 0; i < 1000; i++)
    {
        // 获取当前时间，作为加密开始时间
        gettimeofday(&start, NULL);
        // 调用AES-CBC加密函数
        aes_cbc_encrypt(plaintext, key, iv, ciphertext);
        // 获取当前时间，作为加密结束时间
        gettimeofday(&end, NULL);
        double t = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        sum1 += t;
    }
    sum1 = sum1 * 0.000001;
    // 计算加密的平均时间，单位为微秒
    average_time[0] = sum1;
    // 重置总时间变量为0
    total_time = 0;
    // 循环执行1000次AES-CBC解密
    for (i = 0; i < 1000; i++)
    {
        // 获取当前时间，作为解密开始时间
        gettimeofday(&start, NULL);
        // 调用AES-CBC解密函数
        aes_cbc_decrypt(ciphertext, key, iv, decrypted);
        // 获取当前时间，作为解密结束时间
        gettimeofday(&end, NULL);
        double t = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        sum2 += t;
    }
    sum2 = sum2 * 0.000001;
    // 计算解密的平均时间，单位为微秒
    average_time[1] = sum2;
    // 返回平均时间数组的指针
    return average_time;
}

// 定义主函数，用于测试
int main()
{
    // 调用测试函数，获取平均时间数组的指针
    double *average_time = test_aes_cbc();
    // 输出加密和解密的平均时间
    std::cout << "The average time of AES-CBC encryption is " << average_time[0] << " microseconds." << std::endl;
    std::cout << "The average time of AES-CBC decryption is " << average_time[1] << " microseconds." << std::endl;
    // 返回0，表示程序正常结束
    return 0;
}
