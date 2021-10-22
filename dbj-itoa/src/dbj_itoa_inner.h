#pragma once
/******************************************************************************
2021 by dbj.org
this was not my code, but I could not find the original author

function names should be self explanatory, if you are a C programer :)

dbj_itoa() is extremely useful for me. It is not in the C++ standard, for example.

There are several problems:

- It is stuck with radix 10: Used 99% of the time.
- Possible buffer overflow: Just allocate 12 - twelve - chars.
- It can be emulated with std::stringstream.str() : Hundred times slower.
- It needs 40kbytes of static buffer: Cost=0.001 pence of RAM, or 0.00001 of HD.

This library assumes:

- There is only one way to display numbers. No locale, base 10 only.
- Buffers are big enough.

******************************************************************************/

#include <stdlib.h>
// #include <memory.h>
#include <limits.h>
// #include <stdio.h>
#include <math.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

// #define LONG_DIGITS 10
// #define LLONG_DIGITS 20
#ifndef SAFE_BUF_LEN
#define SAFE_BUF_LEN 0xF
#endif // ! SAFE_BUF_LEN

	unsigned int ilog_10(unsigned int);

	unsigned int lllog_10(unsigned long long);

	char *ustoa(char[static SAFE_BUF_LEN], unsigned short);

	char *stoa(char[static SAFE_BUF_LEN], short);

	char *uitoa(char[static SAFE_BUF_LEN], unsigned int);

	char *uitoa2(char[static SAFE_BUF_LEN], unsigned int);

	char *itoa_dbj(char[static SAFE_BUF_LEN], int);

	char *ulltoa(char[static SAFE_BUF_LEN], unsigned long long);

	char *ulltoa2(char[static SAFE_BUF_LEN], unsigned long long);

	char *ulltoa3(char[static SAFE_BUF_LEN], unsigned long long);

	char *ulltoa4(char[static SAFE_BUF_LEN], unsigned long long);

	char *lltoa(char[static SAFE_BUF_LEN], long long);

	char *dtoa(char[static SAFE_BUF_LEN], double);
#ifdef __cplusplus
};
#endif /* __cplusplus */
