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

#ifdef __cplusplus
} // extern "C"
#endif

DBJ_RESTORE_4996
