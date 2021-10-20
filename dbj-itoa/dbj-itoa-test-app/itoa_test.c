#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>

#include "dbj_itoa.h"

static void full_test_i(const char *msg, char *(func)(char *, unsigned int), int nbLoop)
{
	int nbDigs;
	printf("Integer: %s\n", msg);

	for (nbDigs = 1; nbDigs <= LONG_DIGITS; ++nbDigs)
	{
		clock_t timBeg = clock();

		/* All the number will have the same number of digits. */
		double numBeg = pow(10.0, nbDigs - 1);
		unsigned int numEnd = (unsigned int)(numBeg * 10);

		do
		{
			char tmpBuf[100];
			unsigned int numData = (unsigned int)numBeg;

			int tmpLoop = nbLoop;
			while (tmpLoop--)
			{
				(*func)(tmpBuf, numData);
			};

			unsigned int n2;
			if (1 != sscanf(tmpBuf, "%u", &n2))
			{
				printf("Cannot sscanf\n");
				return;
			};
			if (n2 != numData)
			{
				printf("Err: Buf=[%s] numData=%u n2=%u.\n", tmpBuf, numData, n2);
				return;
			};
			numBeg *= 1.1;
		} while ((numBeg < numEnd) && (numBeg < (double)LONG_MAX));

		clock_t timEnd = clock();
		printf("NbDigits=%02d time=%8ld\n", nbDigs, (timEnd - timBeg) / 1000);
	}
};

#ifndef LLONG_MAX
#define LLONG_MAX (~(long long)0 >> 1)
#endif

static void full_test_ll(const char *msg, char *(func)(char *, unsigned long long), int nbLoop)
{
	int nbDigs;
	printf("LongLong: %s\n", msg);

	for (nbDigs = 1; nbDigs <= LLONG_DIGITS; ++nbDigs)
	{
		clock_t timBeg = clock();

		/* All the number will have the same number of digits. */
		double numBeg = pow(10.0, nbDigs - 1);
		unsigned long long numEnd = (unsigned long long)(numBeg * 10);

		do
		{
			char tmpBuf[100];
			unsigned long long numData = (unsigned long long)numBeg;

			int tmpLoop = nbLoop;
			while (tmpLoop--)
			{
				(*func)(tmpBuf, numData);
			};

			unsigned long long n2;
			if (1 != sscanf(tmpBuf, "%llu", &n2))
			{
				printf("Cannot sscanf\n");
			};
			if (n2 != numData)
			{
				printf("Err: Buf=[%s] N=%lld, n2=%llu.\n", tmpBuf, numData, n2);
				abort();
			};
			numBeg *= 1.01;
		} while ((numBeg < numEnd) && (numBeg < (double)LLONG_MAX));

		clock_t timEnd = clock();
		printf("NbDigits=%02d time=%8ld\n", nbDigs, (timEnd - timBeg) / 1000);
	}
};

static char *sprintf_i(char *ptrBuf, unsigned int uInt)
{
	return ptrBuf + sprintf(ptrBuf, "%u", uInt);
};

static char *sprintf_ll(char *ptrBuf, unsigned long long uLL)
{
	return ptrBuf + sprintf(ptrBuf, "%llu", uLL);
};

static char *(*dummy)(char *, unsigned long long) = sprintf_ll;

static void test(void)
{
	unsigned long long i = 1, s;
	int nbDigs = 1;
	for (nbDigs = 1; nbDigs <= LLONG_DIGITS; ++nbDigs)
	{
		s = 10 * i;
		unsigned long long j;
		printf("i=%llu d=%d\n", i, nbDigs);
		fflush(stdout);
		for (j = i; j < s; j += 100000000000ULL)
		{
			int n = lllog_10(j);
			if (n != nbDigs)
			{
				printf("BAD i=%llu d=%d n=%d\n", j, nbDigs, n);
				break;
			};
		}
		i = s;
	};
};

int main(int argC, const char **argV)
{
	if (argC < 2)
	{
		printf("\n%s\nProvide one of the three commands : x|i|l\n", argV[0]);
		printf("Commands i or l do require no of iterations as a number parameter : i|l [0-9]+\n");
		printf("Commands are not case sensitive\n");
		return EXIT_FAILURE;
	};

	int nb = argC > 2 ? atoi(argV[2]) : 0;

	if (nb < 1)
	{
		printf("\n%s\nProvide number of iterations as parameter to i or l commands: i|l [0-9]+\n", argV[0]);
		return EXIT_FAILURE;
	}

	switch (argV[1][0])
	{
	case 'x':
	case 'X':
		test();
		break;
	case 'i':
	case 'I':
	{
		full_test_i("sprintf_i", sprintf_i, nb);
		full_test_i("uitoa2", uitoa2, nb);
		full_test_i("uitoa", uitoa, nb);
	};
	break;
	case 'l':
	case 'L':
	{
		full_test_ll("sprintf_ll", sprintf_ll, nb);
		full_test_ll("ulltoa", ulltoa, nb);
		full_test_ll("ulltoa2", ulltoa2, nb);
		full_test_ll("ulltoa3", ulltoa3, nb);
		full_test_ll("ulltoa4", ulltoa4, nb);
	};
	break;
	default:
		printf("Bad type selector:%c\n", argV[1][0]);
		return EXIT_FAILURE;
	};
	printf("Test OK\n");
	return EXIT_SUCCESS;
}
