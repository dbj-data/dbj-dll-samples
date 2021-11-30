
#include "../dbj-itoa.h"            // DLL interface
#include "../../dbj-light-loader.h" // DBG_PRINT and the rest

/* ---------------------------------------------------------------------------- */
#include <stdio.h>
#include <limits.h>

#define SX(F_, X_) DBG_PRINT("\n%s : " F_, (#X_), (X_))
/*
The interface:

struct dbj_itoa_
{
    char const *(*decimal_from)(long, const unsigned, char[]);
} ;
*/

// called from main() in dbj-itoa_test.c
int test_decimal_from(dbj_itoa *interface_)
{
    DBJ_TYPE_OF(interface_->decimal_from)
    decimal_from_ = interface_->decimal_from;
    // otherwise it is  uglier and requires peeking into the header
    // char const *(*decimal_from)(long, const unsigned, char[]) = interface_->decimal_from;

    // 512 is huge, 0xF is quite enough
#undef BUFSIZ
#define BUFSIZ 0xF

    char buffy[BUFSIZ] = {0};

    DBG_PRINT("\n%s", "decimal_from()");

    SX("'%s'", decimal_from_(LONG_MAX, BUFSIZ, buffy));
    SX("'%s'", decimal_from_(LONG_MIN, BUFSIZ, buffy));
    SX("'%s'", decimal_from_(0, BUFSIZ, buffy));
    SX("'%s'", decimal_from_(+42L, BUFSIZ, buffy));
    SX("'%s'", decimal_from_(-0L, BUFSIZ, buffy));

    return 42;
}
