
#include "../dbj-itoa.h"
#include "../../dbj-light-loader.h"

/* ---------------------------------------------------------------------------- */
#include <stdio.h>
#include <limits.h> // std::numeric_limits

#define SX(F_, X_) DBG_PRINT("\n%s : " F_, (#X_), (X_))
/*
struct dbj_itoa_
{
    char const *(*decimal_from)(long, const unsigned, char[]);
} ;
*/

// int test_decimal_from()
int test_decimal_from(dbj_itoa *interface_)
{
    // DBG_PRINT("\n%s", "Reversals");
    // {
    //     char specimen[] = "0123456789";
    //     DBG_PRINT("\nBefore:\t%s", specimen);
    //     char *left = specimen;
    //     char *right = specimen + 10;
    //     DBJ_REVERSE(left, right);
    //     DBG_PRINT("\nAfter:\t%s", specimen);
    // }

    // {
    //     int specimen[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    //     DBG_PRINT("\nBefore:");
    //     for (int k = 0; k < 10; ++k)
    //         DBG_PRINT("%3d", specimen[k]);

    //     int *left = specimen;
    //     int *right = specimen + 10;

    //     DBJ_REVERSE(left, right);

    //     DBG_PRINT("\nAfter:");
    //     for (int k = 0; k < 10; ++k)
    //         DBG_PRINT("%3d", specimen[k]);
    // }

    // 20+ years GCC extension not in the ISO C standard; yet
    __typeof__(interface_->decimal_from) decimal_from = interface_->decimal_from;

    // char const *(*decimal_from)(long, const unsigned, char[]) = interface_->decimal_from;

    // 512 is huge
#undef BUFSIZ
// 0xF is quite enough
#define BUFSIZ 0xF

    char buffy[BUFSIZ] = {0};

    DBG_PRINT("\n%s", "decimal_from()");

    SX("'%s'", decimal_from(LONG_MAX, BUFSIZ, buffy));
    SX("'%s'", decimal_from(LONG_MIN, BUFSIZ, buffy));
    SX("'%s'", decimal_from(0, BUFSIZ, buffy));
    SX("'%s'", decimal_from(+42L, BUFSIZ, buffy));
    SX("'%s'", decimal_from(-0L, BUFSIZ, buffy));

    return 42;
}
