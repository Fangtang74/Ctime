/*Author: Lu Wei" */
#include "big.h"
#include "flash.h"
#include "miracl.h"
#include <iostream>
#include "zzn.h"
#include <ctime>
#include <sys/time.h>
#include "zzn.cpp"
#include "big.cpp"
#include "flash.cpp"
#include <random>

Miracl precision(3072, 2);

// ��������ַ���
string generate_random_string(size_t length)
{
	const char charset[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"; // ���Ը�����Ҫ���������ַ�
	string random_string;
	size_t charset_size = sizeof(charset) - 1;
	// ��������ַ���
	while (length--)
	{
		size_t index = rand() % charset_size; // ���ַ��������ѡ��һ���ַ�
		random_string += charset[index];	  // ��ѡ����ַ���ӵ��ַ�����ĩβ
	}
	return random_string;
}

void aes_cbc_one()
{
	aes a;
	char iv[16] = {0, 0, 0, 0,
				   0, 0, 0, 0,
				   0, 0, 0, 0,
				   0, 0, 0, 0};

	Big key_big = rand(16 * 8, 2);
	char key[16];
	to_binary(key_big, 16, key);
	// AES128-CBC
	aes_init(&a, MR_CBC, 16, key, iv);
	string my_random_string = generate_random_string(64); // ����һ������Ϊ10������ַ���
	char *text = my_random_string.data();
	// char text[32] = "helloworld";
	//  cout << "plaintext:"<<text<<endl;
	aes_encrypt(&a, text);
	aes_decrypt(&a, text);
	aes_end(&a);
	// cout << "ciphertext:"<<text<<endl;
}

void measure_aes()
{
	double sum = 0, t = 0;
	struct timeval start, end;
	for (int i = 0; i < 1000; i++)
	{
		gettimeofday(&start, NULL);
		aes_cbc_one();
		gettimeofday(&end, NULL);
		double t = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
		sum += t;
	}
	sum = sum * 0.000001;
	cout << "==========AES-CBC Time==========" << endl;
	cout << endl;
	cout << "AES-CBC runs in ................." << sum << " ms" << endl;
	cout << endl;
}

int main()
{
	measure_aes();
	return 0;
}
