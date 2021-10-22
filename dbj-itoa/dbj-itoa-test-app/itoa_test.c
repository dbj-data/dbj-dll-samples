#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>

#include "../dbj-itoa.h"
#include "../../dbj-light-loader.h"

#ifndef LONG_DIGITS
#define LONG_DIGITS 10
#define LLONG_DIGITS 20
#endif // ! LONG_DIGITS

static void full_test_i(const char *msg, char *(func)(char *, unsigned int), int nbLoop)
{
	int nbDigs;
	DBG_PRINT("Testing conversion of int's, using: %s\n", msg);

	for (nbDigs = 1; nbDigs <= LONG_DIGITS; ++nbDigs)
	{
		// clock_t timBeg = clock();

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
				DBG_PRINT("Cannot sscanf\n");
				return;
			};
			if (n2 != numData)
			{
				DBG_PRINT("Err: Buf=[%s] numData=%u n2=%u.\n", tmpBuf, numData, n2);
				return;
			};
			numBeg *= 1.1;
		} while ((numBeg < numEnd) && (numBeg < (double)LONG_MAX));

		// clock_t timEnd = clock();
		DBG_PRINT("Finished OK for number of digits: %02d \n", nbDigs);
	}
};

#ifndef LLONG_MAX
#define LLONG_MAX (~(long long)0 >> 1)
#endif

static void full_test_ll(const char *msg, char *(func)(char *, unsigned long long), int nbLoop)
{
	int nbDigs;
	DBG_PRINT("Testing conversions of LongLong, using : %s\n", msg);

	for (nbDigs = 1; nbDigs <= LLONG_DIGITS; ++nbDigs)
	{
		// clock_t timBeg = clock();

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
				DBG_PRINT("Cannot sscanf\n");
			};
			if (n2 != numData)
			{
				DBG_PRINT("Err: Buf=[%s] N=%lld, n2=%llu.\n", tmpBuf, numData, n2);
				abort();
			};
			numBeg *= 1.01;
		} while ((numBeg < numEnd) && (numBeg < (double)LLONG_MAX));

		// clock_t timEnd = clock();
		DBG_PRINT("Finished OK for number of digits: %02d \n", nbDigs);
	}
};

// static char *sprintf_i(char *ptrBuf, unsigned int uInt)
// {
// 	return ptrBuf + sprintf(ptrBuf, "%u", uInt);
// };

// static char *sprintf_ll(char *ptrBuf, unsigned long long uLL)
// {
// 	return ptrBuf + sprintf(ptrBuf, "%llu", uLL);
// };

// static char *(*dummy)(char *, unsigned long long) = sprintf_ll;

static void test_lllog_10(dbj_itoa *interface)
{
	unsigned long long i = 1, s;
	int nbDigs = 1;
	for (nbDigs = 1; nbDigs <= LLONG_DIGITS; ++nbDigs)
	{
		s = 10 * i;
		unsigned long long j;
		DBG_PRINT("i=%llu d=%d\n", i, nbDigs);
		fflush(stdout);
		for (j = i; j < s; j += 100000000000ULL)
		{
			int n = interface->lllog_10(j);
			if (n != nbDigs)
			{
				DBG_PRINT("BAD i=%llu d=%d n=%d\n", j, nbDigs, n);
				break;
			};
		}
		i = s;
	};
};

int itoa_test(dbj_itoa *interface)
{
	int nb = 0xF;

	test_lllog_10(interface);
	// full_test_ll("sprintf_ll", sprintf_ll, nb);
	// full_test_i("sprintf_i", sprintf_i, nb);
	full_test_i("uitoa2", interface->uitoa2, nb);
	full_test_i("uitoa", interface->uitoa, nb);
	full_test_ll("ulltoa", interface->ulltoa, nb);
	full_test_ll("ulltoa2", interface->ulltoa2, nb);
	full_test_ll("ulltoa3", interface->ulltoa3, nb);
	full_test_ll("ulltoa4", interface->ulltoa4, nb);
	return EXIT_SUCCESS;
}
