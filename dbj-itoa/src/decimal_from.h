#pragma once
/*
C++ version benchmarked here:
https://www.zverovich.net/2020/06/13/fast-int-to-string-revisited.html

https://ideone.com/nrQfA8

DBJ: transformed to C11
*/

#include "dbj_clang_ignore_warnings.h"
DBJ_DISABLE_4996

#include "dbj_reverse.h"

#ifdef __cplusplus
extern "C"
{
#endif

	// DBJ: this maybe has some computational problems
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
