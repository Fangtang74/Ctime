#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>

#include "NIST/rng.h"
#include "FE.h"

#define TEST_LOOP 1000

int64_t cpucycles(void)
{
	unsigned int hi, lo;

	__asm__ __volatile__("rdtsc\n\t" : "=a"(lo), "=d"(hi));

	return ((int64_t)lo) | (((int64_t)hi) << 32);
}

void HWT(int8_t *delta, int num)
{
#ifdef PARAM_80
	int cnt = 0;

	memset(delta, 0, PARAMS_N);

	while (cnt < num)
	{
		uint8_t tmp1[2], tmp2;

		randombytes(tmp1, 2);

		tmp2 = tmp1[0] % PARAMS_N;

		if (delta[tmp2] == 0)
		{
			delta[tmp2] = (((tmp1[1] >> 7) & 0x1) << 1) - 1;
			cnt++;
		}
	}
#endif

#ifdef PARAM_128
	int cnt = 0;

	memset(delta, 0, PARAMS_N);

	while (cnt < num)
	{
		uint8_t tmp[2];

		randombytes(tmp, 2);

		if (delta[tmp[0]] == 0)
		{
			delta[tmp[0]] = ((tmp[1] & 0x1) << 1) - 1;
			cnt++;
		}
	}
#endif

#ifdef PARAM_256
	int cnt = 0;

	memset(delta, 0, PARAMS_N);

	while (cnt < num)
	{
		uint8_t tmp1[2];
		int16_t tmp2;

		randombytes(tmp1, 2);

		tmp2 = ((tmp1[1] & 0x1) << 8) | tmp1[0];

		if (delta[tmp2] == 0)
		{
			delta[tmp2] = (((tmp1[1] >> 7) & 0x1) << 1) - 1;
			cnt++;
		}
	}
#endif
}

void TEST0()
{
	printf("bio data size : %d BYTES\n", PARAMS_N);
	printf("helper data size : %d BYTES\n", 32 + PARAMS_M);
	printf("extracted key size : %d BYTES\n", PARAMS_R_BYTES);
}

void TEST1()
{

	int cnt = 0;
	uint8_t seed_a[32];
	int8_t c[PARAMS_M] = {0};

	uint8_t r1[PARAMS_R_BYTES] = {0};
	uint8_t r2[PARAMS_R_BYTES] = {0};

	int8_t w[PARAMS_N] = {0};
	int8_t w_prime[PARAMS_N] = {0};
	int8_t delta[PARAMS_N] = {0};
	float error_rate = 0.3;

	// 80-bit
	// 31% => 1/10000
	// 32% => 3/10000
	// 33% => 18/10000
	// 34% => 48/10000
	// 35% => 109/10000
	// 36% => 177/10000
	// 37% => 343/10000
	// 38% => 515/10000
	// 39% => 896/10000

	// 128-bit
	// 21% => 0/10000
	// 22% => 15/10000
	// 23% => 35/10000
	// 24% => 223/10000
	// 25% => 754/10000
	// 26% => 1516/10000
	// 27% => 3082/10000
	// 28% => 4358/10000
	// 29% => 4358/10000

	// 256-bit
	// 10% => 0/10000
	// 11% => 2/10000
	// 12% => 71/10000
	// 13% => 1226/10000
	// 14% => 5372/10000

	printf("==========TEST1 : Correctness==========\n");
	printf("error rate : %f\n", error_rate);
	// Generate bioinfo w
	randombytes(w, PARAMS_N);

	for (int i = 0; i < TEST_LOOP; ++i)
	{
		if (i % 1000 == 0)
			printf("index : %d\n", i);

		// Generate Delta
		HWT(delta, PARAMS_N * error_rate);

		// Generate bioinfo w_prime
		for (int i = 0; i < PARAMS_N; ++i)
		{
			w_prime[i] = w[i] + delta[i];
		}

		Gen(seed_a, c, r1, w);
		Rep(r2, seed_a, c, w_prime);

		if (memcmp(r1, r2, PARAMS_R_BYTES) != 0)
		{
			cnt++;
		}
	}

	printf("count : %d\n", cnt);
}

void TEST2()
{
	int cnt = 0;
	uint8_t seed_a[32];
	int8_t c[PARAMS_M] = {0};

	uint8_t r1[PARAMS_R_BYTES] = {0};
	uint8_t r2[PARAMS_R_BYTES] = {0};

	int8_t w[PARAMS_N] = {0};
	int8_t w_prime[PARAMS_N] = {0};
	int8_t delta[PARAMS_N] = {0};

	printf("==========TEST2 : Soundness==========\n");

	// Generate bioinfo w
	randombytes(w, PARAMS_N);

	for (int i = 0; i < TEST_LOOP; ++i)
	{
		if (i % 1000 == 0)
			printf("index : %d\n", i);

		Gen(seed_a, c, r1, w);

		randombytes(w_prime, PARAMS_N);
		Rep(r2, seed_a, c, w_prime);

		if (memcmp(r1, r2, PARAMS_R_BYTES) != 0)
		{
			cnt++;
		}
	}

	printf("count : %d\n", cnt);
}
void TEST3()
{

	int cnt = 0;
	uint8_t seed_a[32];
	int8_t c[PARAMS_M] = {0};

	uint8_t r1[PARAMS_R_BYTES] = {0};
	uint8_t r2[PARAMS_R_BYTES] = {0};

	int8_t w[PARAMS_N] = {0};
	int8_t delta[PARAMS_N] = {0};
	struct timeval start, end;
	double sum1, sum2 = 0;
	printf("==========Fuzzy Extractor Time==========\n");
	randombytes(w, PARAMS_N);

	for (int i = 0; i < TEST_LOOP; i++)
	{
		gettimeofday(&start, NULL);
		Gen(seed_a, c, r1, w);
		gettimeofday(&end, NULL);
		double t = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
		sum1 += t;
	}
	sum1 = sum1 * 0.000001;

	for (int i = 0; i < TEST_LOOP; i++)
	{
		gettimeofday(&start, NULL);
		Rep(r2, seed_a, c, w);
		gettimeofday(&end, NULL);
		double t = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
		sum2 += t;
	}
	sum2 = sum2 * 0.000001;
	double sum = sum1 + sum2;
	printf("Fuzzy extractor runs in ................. %8f ms\n", sum1);
}

int main(void)
{
	// TEST0();
	//   TEST1();
	//   TEST2();
	TEST3();

	return 0;
}