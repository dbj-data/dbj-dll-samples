#pragma once
/*
C++ std:: transformed to C11
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
    // DBJ: why is this not a macro?
    static inline void jens_g_swap_detail(void *p1, void *p2, void *tmp, size_t pSize)
    {
        memcpy(tmp, p1, pSize);
        memcpy(p1, p2, pSize);
        memcpy(p2, tmp, pSize);
    }

/*
that -1 bellow will stop the compilation if a and b are of different sizes
*/
#define JENS_G_SWAP(a, b)                                                \
    jens_g_swap_detail(                                                  \
        &(a),                                                            \
        &(b),                                                            \
        (char[(sizeof(a) == sizeof(b)) ? (ptrdiff_t)sizeof(a) : -1]){0}, \
        sizeof(a))

    // std::iter swap as C macro
#define DBJ_ITER_SWAP(a, b) JENS_G_SWAP(*a, *b)

// std::reverse in C
// (c) 2021 by dbj@dbj.org
// NOTE: first and last must be real pointers not undecayed arrays
#define DBJ_REVERSE(first, last)                     \
    do                                               \
    {                                                \
        while ((first != last) && (first != --last)) \
        {                                            \
            DBJ_ITER_SWAP(first++, last);            \
        }                                            \
    } while (0)

#ifdef DBJ_REVERSE_TEST

    static inline void dbj_reverse_test(void)
    {
        DBG_PRINT("\n%s", "DBJ Reversals");
        {
            char specimen[] = "0123456789";
            DBG_PRINT("\nBefore:\t%s", specimen);
            char *left = specimen;
            char *right = specimen + 10;
            DBJ_REVERSE(left, right);
            DBG_PRINT("\nAfter:\t%s", specimen);
        }

        {
            int specimen[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
            DBG_PRINT("\nBefore:");
            for (int k = 0; k < 10; ++k)
                DBG_PRINT("%3d", specimen[k]);

            int *left = specimen;
            int *right = specimen + 10;

            DBJ_REVERSE(left, right);

            DBG_PRINT("\nAfter :");
            for (int k = 0; k < 10; ++k)
                DBG_PRINT("%3d", specimen[k]);
        }
    }

#endif // DBJ_REVERSE_TEST

#ifdef __cplusplus
} // extern "C"
#endif

DBJ_RESTORE_4996

/*
Usage example:

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
		}
	}
*/
