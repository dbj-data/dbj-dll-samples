#pragma once
/*
C++ version benchmarked here:
https://www.zverovich.net/2020/06/13/fast-int-to-string-revisited.html

https://ideone.com/nrQfA8

DBJ: transformed to C11
*/

#include "dbj_clang_ignore_warnings.h"
DBJ_DISABLE_4996

#include <string.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

	// https://stackoverflow.com/a/3983089/10870835
	static inline void jens_g_swap_detail(void *p1, void *p2, void *tmp, size_t pSize)
	{
		memcpy(tmp, p1, pSize);
		memcpy(p1, p2, pSize);
		memcpy(p2, tmp, pSize);
	}

#define JENS_G_SWAP(a, b)                                                \
	jens_g_swap_detail(                                                  \
		&(a),                                                            \
		&(b),                                                            \
		(char[(sizeof(a) == sizeof(b)) ? (ptrdiff_t)sizeof(a) : -1]){0}, \
		sizeof(a))

	// std::iter swap as C macro
#define DBJ_ITER_SWAP(a, b) JENS_G_SWAP(*a, *b)

// std::reverse in C
// NOTE: first and last must be real pointers not undecayed arrays
#define DBJ_REVERSE(first, last)                     \
	do                                               \
	{                                                \
		while ((first != last) && (first != --last)) \
		{                                            \
			DBJ_ITER_SWAP(first++, last);            \
		}                                            \
	} while (0)

	// DBJ: this has some computational problems
	// but it is fast
	static inline void unsigned_to_decimal(unsigned long number, char *buffer)
	{
		if (number == 0)
		{
			*buffer++ = '0';
			*buffer = '\0';
		}
		else
		{
			char *p_first = buffer;
			while (number != 0)
			{
				*buffer++ = '0' + number % 10L;
				number /= 10L;
			}
			*buffer = '\0';
			DBJ_REVERSE(p_first, buffer);
			// strrev(p_first);
		}
	}

	static inline char const *decimal_from_unsigned(unsigned long number, char *buffer)
	{
		unsigned_to_decimal(number, buffer);
		return buffer;
	}

	static inline void to_decimal(long number, char *buffer)
	{
		if (number < 0)
		{
			buffer[0] = '-';
			unsigned_to_decimal(-number, buffer + 1);
		}
		else
		{
			unsigned_to_decimal(number, buffer);
		}
	}

	static inline char const *dbj_decimal_from(long number, const unsigned count, char buffer[static count])
	{
		to_decimal(number, buffer);
		return buffer;
	}

// #define DECIMAL_FROM_TEST_
#ifdef DECIMAL_FROM_TEST_
#include <stdio.h>
#include <limits.h> // std::numeric_limits

#define SX(F_, X_) printf("\n%s : " F_, (#X_), (X_))

	int test_decimal_from(const int argc, char *argv[])
	{
		printf("\n%s", "Reversals");
		{
			char specimen[] = "0123456789";
			printf("\nBefore:\t%s", specimen);
			char *left = specimen;
			char *right = specimen + 10;
			DBJ_REVERSE(left, right);
			printf("\nAfter:\t%s", specimen);
		}

		{
			int specimen[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
			printf("\nBefore:");
			for (int k = 0; k < 10; ++k)
				printf("%3d", specimen[k]);

			int *left = specimen;
			int *right = specimen + 10;

			DBJ_REVERSE(left, right);

			printf("\nAfter:");
			for (int k = 0; k < 10; ++k)
				printf("%3d", specimen[k]);
		}

		char spec[BUFSIZ] = {0};

		printf("\n%s", "decimal_from()");

		SX("'%s'", decimal_from(LONG_MAX, spec));
		SX("'%s'", decimal_from(LONG_MIN, spec));
		SX("'%s'", decimal_from(0, spec));
		SX("'%s'", decimal_from(+42L, spec));
		SX("'%s'", decimal_from(-0L, spec));

		return 42;
	}
#endif // DECIMAL_FROM_TEST_

#ifdef __cplusplus
} // extern "C"
#endif

DBJ_RESTORE_4996
